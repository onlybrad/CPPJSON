#include <cstring>
#include "parser.hpp"
#include "lexer.hpp"
#include "util.hpp"

namespace CJSON {

Data::Data() : null(nullptr) {}
Data::~Data() {}

JSON_Node::JSON_Node(JSON_Node &&other) : m_type(other.m_type) {
    if(this != &other) {
        std::memcpy((unsigned char*)&m_value, (unsigned char*)&other.m_value, sizeof(Data));
        other.m_type = Type::NULL_T;
        other.m_value.null = nullptr;
    }
}

JSON_Node::~JSON_Node() {
    switch(m_type) {
    case Type::OBJECT:
        m_value.object.~Object(); break;
    case Type::ARRAY:
        m_value.array.~Array(); break;
    case Type::STRING:
        m_value.string.~basic_string(); break;
    default:;
    }

    m_type = Type::NULL_T;
    m_value.null = nullptr;
}

JSON::JSON() {}

std::string JSON_Node::parseUtf8String(const Token &token, bool &success) {
    std::string string;
    string.reserve(token.value.size - 1U);

    unsigned int index = 1U;
    bool escaping = false;

    while(index < token.value.size - 1U) {
        switch(token.value[index]) {
            case '\b':
            case '\f':
            case '\n':
            case '\r':
            case '\t':
                success = false;
                return string;
        }

        if(!escaping) {
            if(token.value[index] == '\\') {
                escaping = true;
                index++;
                continue;
            }
            
            string.push_back(token.value[index++]);
            continue;
        } 
        
        else switch(token.value[index]) {
        case '"':
            string.push_back('"');
            index++;
            escaping = false;
            continue;
        case 'b':
            string.push_back('\b');
            index++;
            escaping = false;
            continue;
        case 'f':
            string.push_back('\f');
            index++;
            escaping = false;
            continue;
        case 'n':
            string.push_back('\n');
            index++;
            escaping = false;
            continue;
        case 'r':
            string.push_back('\r');
            index++;
            escaping = false;
            continue;
        case 't':
            string.push_back('\t');
            index++;
            escaping = false;
            continue;
        case '/':
            string.push_back('/');
            index++;
            escaping = false;
            continue;
        case '\\': {
            string.push_back('\\');
            index++;
            escaping = false;
            continue;
        }
        case 'u': {
            if(index + 4U >= token.value.size - 1U) {
                success = false;
                return string;
            }

            index++;

            const uint16_t codepoint = parseCodepoint(token.value + index, success);
            //\u0000 is apparently an acceptable escaped character in JSON so if the parsing function returns 0 then it's either an error or \u0000. \0 would break cstrings so it's gonna be treated as an error as well.
            if(codepoint == 0U) {
                success = false;
                return string; 
            }

            if(VALID_2_BYTES_UTF16(codepoint)) {
                utf16ToUtf8(string, codepoint);
                index += 4U;
                escaping = false;
                continue;
            }

            if(index + 9U >= token.value.size - 1U || token.value[index + 4U] != '\\' || token.value[index + 5U] != 'u') {
                success = false;
                return string; 
            }

            const uint16_t low = parseCodepoint(token.value + index + 6U, success);
            if(!success || !VALID_4_BYTES_UTF16(codepoint, low)) {
                success = false;
                return string; 
            }

            utf16ToUtf8(string, codepoint, low);
            index += 10U;
            escaping = false;
            continue;
        }
        default:
            success = false;
            return string;      
    }
    }

    success = !escaping;

    return string;
}

bool JSON_Node::parseString(Tokens &tokens) {
    const Token &token = tokens.data[tokens.index]; 
    bool success;

    std::string string = parseUtf8String(token, success);
    if(success) {
        makeString();
        m_value.string = std::move(string);
        tokens.index++;
    } else {
        m_type = Type::ERROR;
        m_value.error = Error::STRING_FAILED_TO_PARSE;
    }

    return success;
}

bool JSON_Node::parseNumber(Tokens &tokens) {
    std::string string;
    bool success;
    const Token &token = tokens.data[tokens.index]; 

    string.assign(token.value.data, token.value.size);

    if(token.type == TokenType::FLOAT) {
        const double float64 = parseFloat64(string, success);
        if(!success) {
            m_type = Type::ERROR;
            m_value.error = Error::FLOAT64_FAILED_TO_PARSE;
            return false;
        }

        m_type = Type::FLOAT64;
        m_value.float64 = float64;
    } else if(string[0] == '-') {
        if(token.type == TokenType::SCIENTIFIC_INT) {
            const long double long_double = parseLongDouble(string, success);
            if(!success || long_double < INT64_MIN || long_double > INT64_MAX) {
                m_type = Type::ERROR;
                m_value.error = Error::INT64_FAILED_TO_PARSE;
                return false;
            }

            m_type = Type::INT64;
            m_value.int64 = (int64_t)long_double;
        } else {
            const int64_t int64 = parseInt64(string, success);
            if(!success) {
                m_type = Type::ERROR;
                m_value.error = Error::INT64_FAILED_TO_PARSE;
                return false;     
            }


            m_type = Type::INT64;
            m_value.int64 = int64;
        }
    } else if(token.type == TokenType::SCIENTIFIC_INT) {
        const long double long_double = parseLongDouble(string, success);
        if(!success || long_double > UINT64_MAX) {
            m_type = Type::ERROR;
            m_value.error = Error::UINT64_FAILED_TO_PARSE;
            return false;
        }

        m_type = Type::UINT64;
        m_value.uint64 = (uint64_t)long_double;
    } else {
        const uint64_t uint64 = parseUint64(string, success);
        if(!success) {
            m_type = Type::ERROR;
            m_value.error = Error::UINT64_FAILED_TO_PARSE;
            return false; 
        }

        m_type = Type::UINT64;
        m_value.uint64 = uint64;
    }

    tokens.index++;
    
    return true;
}

void JSON_Node::parseBool(Tokens &tokens) {
    const Token &token = tokens.data[tokens.index]; 

    m_type = Type::BOOL;
    m_value.boolean = token.value[0] == 't';

    tokens.index++;
}

void JSON_Node::parseNull(Tokens &tokens) {
    m_type = Type::NULL_T;
    m_value.null = NULL;

    tokens.index++;
}

bool JSON_Node::parseArray(Tokens &tokens) {
    tokens.index++;

    if(tokens.data.size() == tokens.index) {
        m_type = Type::ERROR;
        m_value.error = Error::ARRAY_FAILED_TO_PARSE;
        return false;
    }

    Array &array = makeArray();
    Token &token = tokens.data[tokens.index];

    if(token.type == TokenType::RBRACKET) {
        tokens.index++;
        return true;
    }

    while(tokens.index + 2U < tokens.data.size()) {
        array.emplace_back();
        JSON_Node &next = array.back();

        if(!next.parseTokens(tokens)) {
            const Error error = next.m_value.error;
            array.~Array();
            m_type = Type::ERROR;
            
            if(error == Error::TOKEN_ERROR) {
                m_value.error = Error::ARRAY_INVALID_VALUE;
            } else {
                m_value.error = error;
            }
            return false;
        }

        token = tokens.data[tokens.index];
 
        if(token.type == TokenType::COMMA) {
            tokens.index++;
            token = tokens.data[tokens.index];
            continue;
        }

        if(token.type == TokenType::RBRACKET) {
            tokens.index++;
            return true;
        }

        array.~Array();
        m_type = Type::ERROR;
        m_value.error = Error::ARRAY_MISSING_COMMA_OR_RBRACKET;
        return false;
    }

    array.~Array();
    m_type = Type::ERROR;
    m_value.error = Error::ARRAY_FAILED_TO_PARSE;
    return false;
}

bool JSON_Node::parseObject(Tokens &tokens) {
    tokens.index++;

    if(tokens.data.size() == tokens.index) {
        m_type = Type::ERROR;
        m_value.error = Error::OBJECT_FAILED_TO_PARSE;
        return false;
    }

    Object &object = makeObject();
    Token &token = tokens.data[tokens.index];

    if(token.type == TokenType::RCURLY) {
        tokens.index++;
        return true;
    }

    while(tokens.index + 4U < tokens.data.size()) {
        if(token.type != TokenType::STRING) {
            object.~Object();
            m_type = Type::ERROR;
            m_value.error = Error::OBJECT_INVALID_KEY;
            return false;
        }

        bool success;
        std::string key = parseUtf8String(token, success);
        if(!success) {
            object.~Object();
            m_type = Type::ERROR;
            m_value.error = Error::OBJECT_INVALID_KEY;
            return false;
        }

        token = tokens.data[tokens.index + 1U];
    
        if(token.type != TokenType::COLON) {
            object.~Object();
            m_type = Type::ERROR;
            m_value.error = Error::OBJECT_MISSING_COLON;
            tokens.index++;
        }

        tokens.index += 2U;

        auto it = object.find(key);
        JSON_Node &node = it != object.end() ? it->second : object[key];

        if(it != object.end()) {
            node.~JSON_Node();
        }

        if(!node.parseTokens(tokens)) {
            const Error error = node.m_value.error;
            object.~Object();
            m_type = Type::ERROR;

            if(error == Error::TOKEN_ERROR) {
                m_value.error = Error::OBJECT_INVALID_VALUE;
            } else {
                m_value.error = error;
            }
        }

        token = tokens.data[tokens.index];

        if(token.type == TokenType::COMMA) {
            tokens.index++;
            token = tokens.data[tokens.index];
            continue;
        }

        if(token.type == TokenType::RCURLY) {
            tokens.index++;
            return true;
        }

        m_type = Type::ERROR;
        m_value.error = Error::ARRAY_MISSING_COMMA_OR_RBRACKET;
        return false;
    }
    
    m_type = Type::ERROR;
    m_value.error = Error::OBJECT_FAILED_TO_PARSE;
    return false; 
}

Array &JSON_Node::makeArray() {
    this->~JSON_Node();
    m_type = Type::ARRAY;
    new (&m_value.array) Array();
    m_value.array.reserve(1 << 3);

    return m_value.array;
}

Object &JSON_Node::makeObject() {
    this->~JSON_Node();
    m_type = Type::OBJECT;
    new (&m_value.object) Object();
    m_value.object.reserve(1 << 3);

    return m_value.object;
}

std::string &JSON_Node::makeString() {
    this->~JSON_Node();
    m_type = Type::STRING;
    new (&m_value.string) std::string();

    return m_value.string;
}

bool JSON_Node::parseTokens(Tokens &tokens) {
    const Token &token = tokens.data[tokens.index]; 

    switch(token.type) {
    case TokenType::STRING: {
        return parseString(tokens);
    }
    case TokenType::INT:
    case TokenType::FLOAT:
    case TokenType::SCIENTIFIC_INT: {
        return parseNumber(tokens);
    }
    case TokenType::BOOL: {
        parseBool(tokens);
        return true;
    }
    case TokenType::NULL_T: {
        parseNull(tokens);
        return true;
    }
    case TokenType::LBRACKET: {
        return parseArray(tokens);
    }
    case TokenType::LCURLY: {
        return parseObject(tokens);
    }
    case TokenType::COLON:
    case TokenType::COMMA:
    case TokenType::RBRACKET:
    case TokenType::RCURLY:
    case TokenType::INVALID:
        m_type = Type::ERROR;
        m_value.error = Error::TOKEN_ERROR;
    }

    return false;
}

std::unique_ptr<JSON> JSON::parse(const std::string &data) {
    if(data.size() == 0 || data.size() > UINT_MAX) {
        return nullptr;
    }

    std::unique_ptr<JSON> root(new JSON());
    Lexer lexer(data);
    Token *token;

    do {
        token = root->m_tokens.nextToken();
    } while(lexer.tokenize(*token));

    if(token->type == TokenType::INVALID) {
        root->m_type = Type::ERROR;
        root->m_value.error = Error::TOKEN_ERROR;
        return root;
    }

    root->parseTokens(root->m_tokens);
    return root;
}

}
