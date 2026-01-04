#include <cassert>
#include <cstring>
#include <limits>
#include <new>

#include "parser.hpp"
#include "util.hpp"
#include "file.hpp"
#include "lexer.hpp"

namespace CPPJSON {

bool Parser::parseToken(JSON &json, Tokens &tokens) noexcept {
    switch(tokens.currentToken->type) {
    case Token::Type::STRING: 
        return parseString(json, tokens);
    
    case Token::Type::INT:
    case Token::Type::FLOAT:
    case Token::Type::SCIENTIFIC_INT: 
        return parseNumber(json, tokens);
    
    case Token::Type::BOOL: 
        parseBool(json, tokens);
        return true;
    
    case Token::Type::NUL: 
        parseNull(json, tokens);
        return true;
    
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
        json.set(JSON::Error::TOKEN);
    }

    return false;
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

bool Parser::parseString(JSON &json, Tokens &tokens) noexcept {
    assert(tokens.currentToken != nullptr);

    String *const string = json.makeString(getStringAllocator()); 
    assert(string != nullptr);
    if(!string->reserve(tokens.currentToken->length - 1U)) {
        json.set(JSON::Error::MEMORY);
        return false;
    }

    if(!decodeStringToken(*string, *tokens.currentToken)) {
        json.set(JSON::Error::STRING);
        return false;
    }

    tokens.currentToken++;
    return true;
}

bool Parser::parseArray(JSON &json, Tokens &tokens) noexcept {
    assert(tokens.currentToken != nullptr);
    
    const unsigned length = tokens.currentToken->length;

    tokens.currentToken++;

    const Token *const lastToken = tokens.data.data() + tokens.data.size() - 1;
    if(tokens.currentToken == lastToken) {
        json.set(JSON::Error::ARRAY);
        return false;
    }
    
    Array *const array = json.makeArray(getArrayAllocator());
    assert(array != nullptr);
    if(!array->reserve(length)) {
        json.set(JSON::Error::MEMORY);
        return false;
    }

    if(tokens.currentToken->type == Token::Type::RBRACKET) {
        tokens.currentToken++;
        return true;
    }

    while(lastToken - tokens.currentToken >= 2) {
        array->push();
        JSON &nextJson = array->unsafeBack();
        if(!parseToken(nextJson, tokens)) {
            if(nextJson.m_value.error == JSON::Error::TOKEN) {
                json.set(JSON::Error::ARRAY_VALUE);
            } else {
                json.set(nextJson.m_value.error);
            }
            return false;
        }

        if(tokens.currentToken->type == Token::Type::COMMA) {
            tokens.currentToken++;
            continue;
        }

        if(tokens.currentToken->type == Token::Type::RBRACKET) {
            tokens.currentToken++;
            return true;
        }

        json.set(JSON::Error::MISSING_COMMA_OR_RCURLY);
        return false;
    }

    json.set(JSON::Error::ARRAY);
    return false;
}

bool Parser::parseObject(JSON &json, Tokens &tokens) noexcept {
    assert(tokens.currentToken != nullptr);

    const unsigned length = tokens.currentToken->length;

    tokens.currentToken++;
    assert(tokens.currentToken != nullptr);
    
    const Token *const lastToken = tokens.data.data() + tokens.data.size() - 1;
    if(tokens.currentToken == lastToken) {
        json.set(JSON::Error::OBJECT);
        return false;
    }

    Object *const object = json.makeObject(getObjectAllocator());
    assert(object != nullptr);
    if(!object->reserve(length)) {
        json.set(JSON::Error::MEMORY);
        return false;
    }

    if(tokens.currentToken->type == Token::Type::RCURLY) {
        tokens.currentToken++;
        return true;
    }

    while(lastToken - tokens.currentToken >= 4) {
        if(tokens.currentToken->type != Token::Type::STRING) {
            json.set(JSON::Error::OBJECT_KEY);
            return false;
        }

        String key(getStringAllocator());
        if(!decodeStringToken(key, *tokens.currentToken)) {
            json.set(JSON::Error::OBJECT_KEY);
            return false;
        }

        tokens.currentToken++;
    
        if(tokens.currentToken->type != Token::Type::COLON) {
            json.set(JSON::Error::MISSING_COLON);
            return false;
        }

        tokens.currentToken++;

        JSON *nextJson = (*object)[std::move(key.getContainer())];
        assert(nextJson != nullptr);
        if(!parseToken(*nextJson, tokens)) {
            if(nextJson->m_value.error == JSON::Error::TOKEN) {
                json.set(JSON::Error::OBJECT_VALUE);
            } else {
                json.set(nextJson->m_value.error);
            }
            return false;
        }

        if(tokens.currentToken->type == Token::Type::COMMA) {
            tokens.currentToken++;
            continue;
        }

        if(tokens.currentToken->type == Token::Type::RCURLY) {
            tokens.currentToken++;
            return true;
        }

        json.set(JSON::Error::MISSING_COMMA_OR_RCURLY);
        return false;
    }
    
    json.set(JSON::Error::OBJECT);
    return false;
}

bool Parser::parseNumber(JSON &json, Tokens &tokens) noexcept {
    assert(tokens.currentToken != nullptr);

    bool success;
    char number[1 << 9] = {};

    Token &token = *tokens.currentToken;
    if(static_cast<std::size_t>(token.length) >= sizeof(number)) {
        return false;
    }

    std::memcpy(number, token.value, static_cast<size_t>(token.length));

    if(token.type == Token::Type::FLOAT) {
        const double value = Util::parseFloat64(number, success);
        if(!success) {
            json.set(JSON::Error::FLOAT64);
            return false;
        }
        json.set(value);
    } else if(number[0] == '-') {
        if(token.type == Token::Type::SCIENTIFIC_INT) {
            const long double value = Util::parseLongDouble(number, success);
            if(!success 
            || value < static_cast<long double>(std::numeric_limits<std::int64_t>::min()) 
            || value > static_cast<long double>(std::numeric_limits<std::int64_t>::max())
            ){
                json.set(JSON::Error::INT64);
                return false;
            }
            json.set(static_cast<std::int64_t>(value));
        } else {
            const std::int64_t value = Util::parseInt64(number, success);
            if(!success) {
                json.set(JSON::Error::INT64);
                return false; 
            }
            json.set(value);
        }
    } else if(token.type == Token::Type::SCIENTIFIC_INT) {
        const long double value = Util::parseLongDouble(number, success);
        if(!success || value > static_cast<long double>(std::numeric_limits<std::uint64_t>::max())) {
            json.set(JSON::Error::UINT64);
            return false;
        }
        json.set(static_cast<std::uint64_t>(value));
    } else {
        const uint64_t value = Util::parseUint64(number, success);
        if(!success) {
            json.set(JSON::Error::UINT64);
            return false;
        }
        json.set(value);
    }

    tokens.currentToken++;
    
    return true;
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

Parser::~Parser() noexcept {}

JSON &Parser::init() noexcept {
    if(m_arenas == nullptr) {
        const std::array<unsigned, 3> arenaSizes = {0U, 0U, 0U};
        if(!initArenas(arenaSizes, Arena::INFINITE_NODES)) {
            m_json.set(JSON::Error::MEMORY);
        }
    }

    return m_json;
}

JSON &Parser::parse(const std::string &data) noexcept {
    assert(data[0] != '\0');

    if(data.size() >= static_cast<std::size_t>(std::numeric_limits<unsigned>::max())) {
        m_json.set(JSON::Error::TOO_LARGE);
        return m_json;
    }

    return parse(data.c_str(), static_cast<unsigned>(data.size()));
}

JSON &Parser::parse(const char *const data) noexcept {
    assert(data != nullptr);
    assert(data[0] != '\0');

    const size_t length = std::strlen(data);
    if(length >= static_cast<std::size_t>(std::numeric_limits<unsigned>::max())) {
        m_json.set(JSON::Error::TOO_LARGE);
        return m_json;
    }

    return parse(data, static_cast<unsigned>(length));
}

JSON &Parser::parse(const char *const data, const unsigned length) noexcept {
    assert(data != nullptr);
    assert(length > 0);

    if(m_arenas != nullptr) {
        m_tokens.reset();
    }

    if(!m_tokens.reserve(length / 2U)) {
        m_json.set(JSON::Error::MEMORY);
        return m_json;
    }
    
    Lexer lexer(data, length);
    const Lexer::Error error = lexer.tokenize(m_tokens, m_counters);
    if(error == Lexer::Error::TOKEN) {
        m_json.set(JSON::Error::TOKEN);
        return m_json;
    }
    if(error == Lexer::Error::MEMORY) {
        m_json.set(JSON::Error::MEMORY);
        return m_json;
    }

    if(m_arenas != nullptr) {
        const std::array<unsigned, 3> arenaCounts = {
            m_counters.object_elements,
            m_counters.array_elements,
            m_counters.chars          
        };

        if(!reserveArenas(arenaCounts)) {
            m_json.set(JSON::Error::MEMORY);
            return m_json;
        }
    } else {
        bool success;
        const unsigned objectSizes = Util::safeMult(
            m_counters.object_elements,
            static_cast<unsigned>(sizeof(Object::ContainerType)),
            success
        );
        if(!success) {
            m_json.set(JSON::Error::TOO_LARGE);
            return m_json;
        }

        const unsigned arraySizes = Util::safeMult(
            m_counters.array_elements,
            static_cast<unsigned>(sizeof(Array::ContainerType)),
            success
        );
        if(!success) {
            m_json.set(JSON::Error::TOO_LARGE);
            return m_json;
        }

        const unsigned stringSizes = Util::safeMult(
            m_counters.chars,
            static_cast<unsigned>(sizeof(String::ContainerType)),
            success
        );
        if(!success) {
            m_json.set(JSON::Error::TOO_LARGE);
            return m_json;
        }

        const std::array<unsigned, 3> arenaSizes = {objectSizes, arraySizes, stringSizes};
        
        if(!initArenas(arenaSizes, Arena::INFINITE_NODES)) {
            m_json.set(JSON::Error::MEMORY);
            return m_json;
        }
    }

    parseToken(m_json, m_tokens);
    return m_json;
}

JSON &Parser::parseFile(const std::string &path) noexcept {
    assert(path[0] != '\0');

    return parseFile(path.c_str());
}

JSON &Parser::parseFile(const char *const path) noexcept {
    assert(path != nullptr);
    assert(path[0] != '\0');

    FileContents fileContents = FileContents::get(path);
    if(fileContents.getError() != FileContents::Error::NONE) {
        m_json.set(JSON::Error::FILE);
        return m_json;
    }

    return parse(reinterpret_cast<const char*>(fileContents.getData()), fileContents.getLength());
}

const char *Parser::getError() noexcept {
    switch(m_json.m_value.error) {
    case JSON::Error::NONE:
        return "No Error.";
    case JSON::Error::TOKEN:
        return "Token error.";
    case JSON::Error::STRING:
        return "String failed to parse.";
    case JSON::Error::FLOAT64:
        return "Float64 failed to parse.";
    case JSON::Error::INT64:
        return "Int64 failed to parse.";
    case JSON::Error::UINT64:
        return "Uint64 failed to parse.";
    case JSON::Error::OBJECT:
        return "Object failed to parse.";
    case JSON::Error::OBJECT_KEY:
        return "Object invalid key.";
    case JSON::Error::OBJECT_VALUE:
        return "Object invalid value.";
    case JSON::Error::MISSING_COLON:
        return "Object missing colon.";
    case JSON::Error::MISSING_COMMA_OR_RCURLY:
        return "Missing comma or right curly bracket.";
    case JSON::Error::ARRAY:
        return "Array failed to parse.";
    case JSON::Error::ARRAY_VALUE:
        return "Array invalid value.";
    case JSON::Error::FILE:
        return "Failed to open file.";
    case JSON::Error::MEMORY:
        return "Failed to allocate memory.";
    case JSON::Error::TOO_LARGE:
        return "File or string too large. Maximum supported is UINT_MAX.";
    }

    return nullptr;
}

void Parser::deallocateArenas(Arenas *const arenas) noexcept {
    assert(arenas != nullptr);

    arenas->object.~Arena();
    arenas->array.~Arena();
    arenas->string.~Arena();

    Allocator::s_deallocate(arenas);
}

bool Parser::allocateArenas() noexcept {
    try {
        Arenas *const arenas = new (GeneralAllocator<Arenas>::s_allocate(1U)) Arenas();
        new (&arenas->object) Arena();
        new (&arenas->array) Arena();
        new (&arenas->string) Arena();
        m_arenas.reset(arenas);
        return true;
    } catch(...) {
        return false;
    }
}

bool Parser::initArenas(const std::array<unsigned, 3> arenaSizes, const unsigned maxNodes) noexcept {
    assert(m_arenas == nullptr);

    if(!allocateArenas()) {
        return false;
    }

    struct ArenaData {
        const char    *name;
        Arena         *arena;
        const unsigned containerSize;
    };

    const std::array<ArenaData, 3> arenaData = {{
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
        }
    }};

    int index = 0;
    for(auto &arenaDatum : arenaData) {
        assert(!Util::checkMultOverflow(Arena::MINIMUM_SIZE, arenaDatum.containerSize));

        const unsigned arenaSize = std::max(arenaSizes[index], Arena::MINIMUM_SIZE * arenaDatum.containerSize);
        if(!arenaDatum.arena->init(arenaSize, maxNodes, arenaDatum.name)) {
            return false;
        }
        index++;
    }

    return true;
}

bool Parser::reserveArenas(const std::array<unsigned, 3> arenaCounts) noexcept {
    assert(m_arenas != nullptr);

    if(!m_arenas->object.reserve<Object::ContainerType>(std::max(arenaCounts[0], Arena::MINIMUM_SIZE))) {
        return false;
    }
    if(!m_arenas->array.reserve<Array::ContainerType>(std::max(arenaCounts[1], Arena::MINIMUM_SIZE))) {
        return false;
    }
    if(!m_arenas->string.reserve<String::ContainerType>(std::max(arenaCounts[2], Arena::MINIMUM_SIZE))) {
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

}