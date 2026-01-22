#include <cassert>
#include <cstring>
#include <limits>
#include <new>

#include "parser.hpp"
#include "util.hpp"
#include "file.hpp"
#include "lexer.hpp"

namespace CPPJSON {

Error Parser::parseToken(JSON &json, Tokens &tokens) noexcept {
    switch(tokens.currentToken->type) {
    case Token::Type::STRING: 
        return parseString(json, tokens);
    
    case Token::Type::INT:
    case Token::Type::FLOAT:
    case Token::Type::SCIENTIFIC_INT: 
        return parseNumber(json, tokens);
    
    case Token::Type::BOOL: 
        parseBool(json, tokens);
        return Error::NONE;
    
    case Token::Type::NUL: 
        parseNull(json, tokens);
        return Error::NONE;
    
    case Token::Type::LBRACKET: 
        return parseArray(json, tokens);
    
    case Token::Type::LCURLY:
        return parseObject(json, tokens);
    
    case Token::Type::COLON:
    case Token::Type::COMMA:
    case Token::Type::RBRACKET:
    case Token::Type::RCURLY:
    case Token::Type::INVALID:
    case Token::Type::DONE:
        return Error::TOKEN;
    }

    return Error::NONE;
}

bool Parser::decodeStringToken(String &str, Token &token) noexcept {
    const char *const input_end     = token.value + token.length - 2;
    const char       *input_current = token.value + 1;
    bool              escaping      = false;

    while(input_current != input_end + 1) {
        switch(*input_current) {
            case '\b':
            case '\f':
            case '\n':
            case '\r':
            case '\t':
                return false;
        }

        if(!escaping) {
            if(*input_current == '\\') {
                escaping = true;
                input_current++;
                continue;
            }

            str.push(*(input_current++));
            continue;
        } 
        
        switch(*input_current) {
        case '"':
            escaping    = false;
            str.push('"');
            input_current++;
            continue;
        case 'b':
            escaping    = false;
            str.push('\b');
            input_current++;
            continue;
        case 'f':
            escaping    = false;
            str.push('\f');
            input_current++;
            continue;
        case 'n':
            escaping    = false;
            str.push('\n');
            input_current++;
            continue;
        case 'r':
            escaping    = false;
            str.push('\r');
            input_current++;
            continue;
        case 't':
            escaping    = false;
            str.push('\t');
            input_current++;
            continue;
        case '/':
            escaping    = false;
            str.push('/');
            input_current++;
            continue;
        case '\\': {
            escaping    = false;
            str.push('\\');
            input_current++;
            continue;
        }
        case 'u': {
            char utf8Str[5];

            if(input_end - input_current < 4) {
                return false;
            }

            input_current++;

            bool success;
            const std::uint16_t high = Util::hexToUtf16(input_current, success);
            //\u0000 is apparently an acceptable escaped character in JSON so if the parsing function returns 0 then it's either an error or \u0000. \0 would break cstrings so it's gonna be treated as an error as well.
            if(high == 0U) {
                return false;    
            }

            if(Util::isValidUtf16(high)) {
                str           += Util::utf16ToUtf8(utf8Str, high);
                input_current += 4;
                escaping       = false;
                continue;
            }

            if(input_end - input_current < 9 || input_current[4] != '\\' || input_current[5] != 'u') {
                return false;
            }

            const std::uint16_t low = Util::hexToUtf16(input_current + 6, success);
            if(!success || !Util::isValidUtf16(high, low)) {
                return false; 
            }
            
            str           += Util::utf16ToUtf8(utf8Str, high, low);
            input_current += 10;
            escaping       = false;
            continue;
        }
        default:
            return false;        
    }
    }

    return !escaping;
}

Error Parser::parseString(JSON &json, Tokens &tokens) noexcept {
    assert(tokens.currentToken != nullptr);

    Result<String&> stringResult = json.makeString(getStringAllocator()); 
    assert(stringResult.isSuccess());
    String &string = stringResult.getRef();
    if(!string.reserve(tokens.currentToken->length - 1U)) {
        return Error::MEMORY;
    }

    if(!decodeStringToken(string, *tokens.currentToken)) {
        return Error::STRING;
    }

    tokens.currentToken++;
    return Error::NONE;
}

Error Parser::parseArray(JSON &json, Tokens &tokens) noexcept {
    assert(tokens.currentToken != nullptr);
    
    const unsigned length = tokens.currentToken->length;

    tokens.currentToken++;

    const Token *const lastToken = tokens.data.data() + tokens.data.size() - 1;
    if(tokens.currentToken == lastToken) {
        return Error::ARRAY;
    }
    
    const Result<Array&> arrayResult = json.makeArray(getArrayAllocator());
    assert(arrayResult.isSuccess());
    Array &array = arrayResult.getRef();
    if(!array.reserve(length)) {
        return Error::MEMORY;
    }

    if(tokens.currentToken->type == Token::Type::RBRACKET) {
        tokens.currentToken++;
        return Error::NONE;
    }

    while(lastToken - tokens.currentToken >= 2) {
        array.push();
        JSON &nextJSON = array.unsafeBack();
        const Error error = parseToken(nextJSON, tokens);
        if(error == Error::TOKEN) {
            return Error::ARRAY_VALUE;
        } 
        if(error != Error::NONE) {
            return error;
        }

        if(tokens.currentToken->type == Token::Type::COMMA) {
            tokens.currentToken++;
            continue;
        }

        if(tokens.currentToken->type == Token::Type::RBRACKET) {
            tokens.currentToken++;
            return Error::NONE;
        }

        return Error::MISSING_COMMA_OR_RBRACKET;
    }

    return Error::ARRAY;
}

Error Parser::parseObject(JSON &json, Tokens &tokens) noexcept {
    assert(tokens.currentToken != nullptr);

    const unsigned length = tokens.currentToken->length;

    tokens.currentToken++;
    assert(tokens.currentToken != nullptr);
    
    const Token *const lastToken = tokens.data.data() + tokens.data.size() - 1;
    if(tokens.currentToken == lastToken) {
        return Error::OBJECT;
    }

    Result<Object&> objectResult = json.makeObject(getObjectAllocator());
    assert(objectResult.isSuccess());
    Object &object = objectResult.getRef();
    if(!object.reserve(length)) {
        return Error::MEMORY;
    }

    if(tokens.currentToken->type == Token::Type::RCURLY) {
        tokens.currentToken++;
        return Error::NONE;
    }

    while(lastToken - tokens.currentToken >= 4) {
        if(tokens.currentToken->type != Token::Type::STRING) {
            return Error::OBJECT_KEY;
        }

        String key(getStringAllocator());
        if(!decodeStringToken(key, *tokens.currentToken)) {
            return Error::OBJECT_KEY;
        }

        tokens.currentToken++;
    
        if(tokens.currentToken->type != Token::Type::COLON) {
            return Error::MISSING_COLON;
        }

        tokens.currentToken++;

        JSON &nextJSON = object[std::move(key)];
        const Error error = parseToken(nextJSON, tokens);
        if(error == Error::TOKEN) {
            return Error::OBJECT_VALUE;
        }
        if(error != Error::NONE) {
            return error;
        }

        if(tokens.currentToken->type == Token::Type::COMMA) {
            tokens.currentToken++;
            continue;
        }

        if(tokens.currentToken->type == Token::Type::RCURLY) {
            tokens.currentToken++;
            return Error::NONE;
        }

        return Error::MISSING_COMMA_OR_RCURLY;
    }
    
    return Error::OBJECT;
}

Error Parser::parseNumber(JSON &json, Tokens &tokens) noexcept {
    assert(tokens.currentToken != nullptr);

    bool success;
    char number[1 << 9] = {};

    Token &token = *tokens.currentToken;
    if(static_cast<std::size_t>(token.length) >= sizeof(number)) {
        return Error::TOO_LARGE;
    }

    std::memcpy(number, token.value, static_cast<size_t>(token.length));

    if(token.type == Token::Type::FLOAT) {
        const double value = Util::parseFloat64(number, success);
        if(!success) {
            return Error::FLOAT64;
        }
        json.set(value);
    } else if(number[0] == '-') {
        if(token.type == Token::Type::SCIENTIFIC_INT) {
            const long double value = Util::parseLongDouble(number, success);
            if(!success 
            || value < static_cast<long double>(std::numeric_limits<std::int64_t>::min()) 
            || value > static_cast<long double>(std::numeric_limits<std::int64_t>::max())
            ){
                return Error::INT64;
            }
            json.set(static_cast<std::int64_t>(value));
        } else {
            const std::int64_t value = Util::parseInt64(number, success);
            if(!success) {
                return Error::INT64;
            }
            json.set(value);
        }
    } else if(token.type == Token::Type::SCIENTIFIC_INT) {
        const long double value = Util::parseLongDouble(number, success);
        if(!success || value > static_cast<long double>(std::numeric_limits<std::uint64_t>::max())) {
            return Error::UINT64;
        }
        json.set(static_cast<std::uint64_t>(value));
    } else {
        const uint64_t value = Util::parseUint64(number, success);
        if(!success) {
            return Error::UINT64;
        }
        json.set(value);
    }

    tokens.currentToken++;
    
    return Error::NONE;
}

void Parser::parseNull(JSON &json, Tokens &tokens) noexcept {
    assert(tokens.currentToken != nullptr);

    json.set();
    tokens.currentToken++;
}

void Parser::parseBool(JSON &json, Tokens &tokens) noexcept {
    assert(tokens.currentToken != nullptr);

    json.set(tokens.currentToken->value[0] == 't');
    tokens.currentToken++;
}

Parser::Parser() noexcept {}

Parser::~Parser() noexcept {
    for(RootNode *current = m_firstRoot; current != nullptr; current = current->next) {
        current->json.~JSON();
    }
}

ParserResult Parser::init() noexcept {
    if(m_arenas == nullptr) {
        const ArenaSizes arenaSizes = {0U, 0U, 0U, 0U};
        if(!initArenas(arenaSizes, Arena::INFINITE_NODES)) {
            return ParserResult::fromError(Error::MEMORY);
        }
    }

    RootNode *const rootNode = newRootNode();
    if(rootNode == nullptr) {
        return ParserResult::fromError(Error::MEMORY);
    }

    return ParserResult::fromRef(rootNode->json);
}

ParserResult Parser::parse(const std::string &data) noexcept {
    assert(data[0] != '\0');

    if(data.size() >= static_cast<std::size_t>(std::numeric_limits<unsigned>::max())) {
        return ParserResult::fromError(Error::TOO_LARGE);
    }

    return parse(data.c_str(), static_cast<unsigned>(data.size()));
}

ParserResult Parser::parse(const char *const data) noexcept {
    assert(data != nullptr);
    assert(data[0] != '\0');

    const size_t length = std::strlen(data);
    if(length >= static_cast<std::size_t>(std::numeric_limits<unsigned>::max())) {
        return ParserResult::fromError(Error::TOO_LARGE);
    }

    return parse(data, static_cast<unsigned>(length));
}

ParserResult Parser::parse(const char *const data, const unsigned length) noexcept {
    assert(data != nullptr);
    assert(length > 0);

    Tokens tokens;
    if(!tokens.reserve(length / 2U)) {
        return ParserResult::fromError(Error::MEMORY);
    }
    
    Lexer lexer(data, length);
    Counters counters;
    const Lexer::Error lexerError = lexer.tokenize(tokens, counters);
    if(lexerError == Lexer::Error::TOKEN) {
        return ParserResult::fromError(Error::TOKEN);
    }
    if(lexerError == Lexer::Error::MEMORY) {
        return ParserResult::fromError(Error::MEMORY);
    }

    if(m_arenas != nullptr) {
        const ArenaSizes arenaCounts = {
            counters.object_elements,
            counters.array_elements,
            counters.chars,
            0U       
        };

        if(!reserveArenas(arenaCounts)) {
            return ParserResult::fromError(Error::MEMORY);
        }
    } else {
        Result<unsigned> result = Util::safeMult(
            counters.object_elements,
            static_cast<unsigned>(sizeof(Object::ContainerType))
        );
        if(!result.isSuccess()) {
            return ParserResult::fromError(Error::TOO_LARGE);
        }
        const unsigned objectSizes = result.getValue();

        result = Util::safeMult(
            counters.array_elements,
            static_cast<unsigned>(sizeof(Array::ContainerType))
        );
        if(!result.isSuccess()) {
            return ParserResult::fromError(Error::TOO_LARGE);
        }
        const unsigned arraySizes = result.getValue();

        result = Util::safeMult(
            counters.chars,
            static_cast<unsigned>(sizeof(String::ContainerType))
        );
        if(!result.isSuccess()) {
            return ParserResult::fromError(Error::TOO_LARGE);
        }
        const unsigned stringSizes = result.getValue();

        const ArenaSizes arenaSizes = {objectSizes, arraySizes, stringSizes, 0U};
        if(!initArenas(arenaSizes, Arena::INFINITE_NODES)) {
            return ParserResult::fromError(Error::MEMORY);
        }
    }

    RootNode *const rootNode = newRootNode();
    if(rootNode == nullptr) {
        return ParserResult::fromError(Error::MEMORY);
    }

    const Error error = parseToken(rootNode->json, tokens);
    if(error != Error::NONE) {
        return ParserResult::fromError(error);
    }
    
    return ParserResult::fromRef(rootNode->json);
}

ParserResult Parser::parseFile(const std::string &path) noexcept {
    assert(path[0] != '\0');

    return parseFile(path.c_str());
}

ParserResult Parser::parseFile(const char *const path) noexcept {
    assert(path != nullptr);
    assert(path[0] != '\0');

    FileContents fileContents = FileContents::get(path);
    if(fileContents.getError() != FileContents::Error::NONE) {
        return ParserResult::fromError(Error::FILE);
    }

    return parse(reinterpret_cast<const char*>(fileContents.getData()), fileContents.getLength());
}

bool Parser::allocateArenas() noexcept {
    try {
        Arenas *const arenas = new (ArenasAllocator::s_allocate(1U)) Arenas();
        m_arenas.reset(arenas);
        return true;
    } catch(...) {
        return false;
    }
}

void Parser::deallocateArenas(Arenas *const arenas) noexcept {
    arenas->string.~Arena();
    arenas->object.~Arena();
    arenas->array.~Arena();
    arenas->root.~Arena();

    ArenasAllocator::s_deallocate(arenas);
}

Parser::RootNode *Parser::newRootNode() noexcept {
    assert(m_arenas != nullptr);

    RootNode *const rootNode = m_arenas->root.alloc<RootNode>(1U);
    if(rootNode == nullptr) {
        return nullptr;
    }
    new (rootNode) RootNode();

    if(m_firstRoot == nullptr) {
        m_firstRoot = rootNode;
    } else {
        m_currentRoot->next = rootNode;
    }

    m_currentRoot = rootNode;

    return rootNode;
}

bool Parser::initArenas(const ArenaSizes arenaSizes, const unsigned maxNodes) noexcept {
    assert(m_arenas == nullptr);

    if(!allocateArenas()) {
        return false;
    }

    struct ArenaDatum {
        const char    *name;
        Arena         *arena;
        const unsigned containerSize;
    };

    const std::array<ArenaDatum, arenaSizes.size()> arenaData = {{
        {
            "Object Arena",
            &m_arenas->object,
            static_cast<unsigned>(sizeof(Object::ContainerType))
        },
        {
            "Array Arena",
            &m_arenas->array,
            static_cast<unsigned>(sizeof(Array::ContainerType))
        },
        {
            "String Arena",
            &m_arenas->string,
            static_cast<unsigned>(sizeof(String::ContainerType))
        },
        {
            "JSON Nodes Arena",
            &m_arenas->root,
            static_cast<unsigned>(sizeof(RootNode))
        }
    }};

    int index = 0;
    for(auto &arenaDatum : arenaData) {
        assert(!Util::checkMultOverflow(Arena::MINIMUM_CAPACITY, arenaDatum.containerSize));

        const unsigned arenaSize = std::max(arenaSizes[index], Arena::MINIMUM_CAPACITY * arenaDatum.containerSize);
        if(!arenaDatum.arena->init(arenaSize, maxNodes, arenaDatum.name)) {
            return false;
        }
        index++;
    }

    return true;
}

bool Parser::reserveArenas(const ArenaSizes arenaCounts) noexcept {
    assert(m_arenas != nullptr);

    int i = 0;

    if(!m_arenas->object.reserve<Object::ContainerType>(std::max(arenaCounts[i++], Arena::MINIMUM_CAPACITY))) {
        return false;
    }
    if(!m_arenas->array.reserve<Array::ContainerType>(std::max(arenaCounts[i++], Arena::MINIMUM_CAPACITY))) {
        return false;
    }
    if(!m_arenas->string.reserve<String::ContainerType>(std::max(arenaCounts[i++], Arena::MINIMUM_CAPACITY))) {
        return false;
    }
    if(!m_arenas->root.reserve<RootNode>(std::max(arenaCounts[i++], Arena::MINIMUM_CAPACITY))) {
        return false;
    }

    return true;
}

Object::Allocator Parser::getObjectAllocator() noexcept {
    return Object::Allocator(&m_arenas->object);
}

Array::Allocator Parser::getArrayAllocator() noexcept {
    return Array::Allocator(&m_arenas->array);
}

String::Allocator Parser::getStringAllocator() noexcept {
    return String::Allocator(&m_arenas->string);
}

String Parser::createString(const std::string &key) { 
    return createString(key.c_str());
}

String Parser::createString(const char *const key) { 
    String string(getStringAllocator());
    string = key;
    return string;
}

}