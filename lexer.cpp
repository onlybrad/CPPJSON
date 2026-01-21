#include <cassert>
#include <cstring>
#include <algorithm>
#include <stack>
#include <vector>

#include "lexer.hpp"
#include "token.hpp"
#include "util.hpp"
#include "array.hpp"
#include "object.hpp"
#include "counters.hpp"

namespace CPPJSON {

bool Lexer::countCountainersElements(Tokens &tokens, Counters &counters) noexcept {
    typedef GeneralAllocator<Token*>       Allocator;
    typedef std::vector<Token*, Allocator> Vector;
    typedef std::stack<Token*, Vector>     Stack;

    Vector vector(0, Allocator());
    try {
        vector.reserve(counters.object + counters.array);
    } catch(...) {
        return false;
    }

    Stack stack(std::move(vector));

    for(Token &token : tokens.data) {
        switch(token.type) {
        case Token::Type::LCURLY:
        case Token::Type::LBRACKET: {
            stack.push(&token);
            continue;
        }

        case Token::Type::RCURLY:
        case Token::Type::RBRACKET: {
            if(stack.empty()) {
                return false;
            }

            Token *const container = stack.top();
            assert(container != nullptr);
            assert(container->type == Token::Type::LCURLY || container->type == Token::Type::LBRACKET);
            stack.pop();

            if(container->type == Token::Type::LCURLY) {
                counters.object_elements += std::max(container->length, Object::MINIMUM_CAPACITY);
            } else {
                counters.array_elements += std::max(container->length, Array::MINIMUM_CAPACITY);
            }

            continue;
        }

        case Token::Type::COMMA: {
            if(stack.empty()) {
                return false;
            }

            Token *const container = stack.top();
            assert(container != nullptr);
            assert(container->type == Token::Type::LCURLY || container->type == Token::Type::LBRACKET);

            container->length++;

            continue;
        }

        default:
            continue;
        }
    }

    return true;
}

Lexer::Lexer(const char *const data, const unsigned length) noexcept :
    m_data(data),
    m_length(length)
{}

void Lexer::skipWhitespace() noexcept {
    while(m_position < m_length && Util::isWhitespace(m_data[m_position])
    ) {
        m_position++;
    }
}

Lexer::Error Lexer::tokenize(Tokens &tokens, Counters &counters) noexcept {
    skipWhitespace();

    while(m_position < m_length) {
        Token *const token = tokens.nextToken();
        if(token == nullptr) {
            return Error::MEMORY;
        }

        token->value = m_data + m_position;
        switch(*token->value) {
        case '{':
            token->length = 1U;
            token->type   = Token::Type::LCURLY;
            break;
        case '}':
            token->length = 1U;
            token->type   = Token::Type::RCURLY;
            counters.object++;
            break;
        case '[':
            token->length = 1U;
            token->type   = Token::Type::LBRACKET;
            break;
        case ']':
            token->length = 1U;
            token->type   = Token::Type::RBRACKET;
            counters.array++;
            break;
        case ':':
            token->length = 1U;
            token->type   = Token::Type::COLON;
            break;
        case ',': {
            token->length = 1U;
            token->type   = Token::Type::COMMA;
            counters.comma++;
            break;
        }
        case '"': {
            if(!readString(*token)) {
                return Error::TOKEN;
            }
            assert(token->length >= 2U);
            counters.string++;
            counters.chars += token->length - 1U;
            break;
        }
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            if(!readNumber(*token)) {
                 return Error::TOKEN;
            }
            assert(token->length >= 1U);
            counters.number++;
            break;
        }
        default: {
            if(!readKeyword(*token)) {
                readInvalidToken(*token);
                return Error::TOKEN;
            }
            assert(token->length >= 4U);
            counters.keyword++;
            break;
        }

        }
        
        m_position += token->length;
        skipWhitespace();
    }

    Token *const token = tokens.nextToken();
    if(token == nullptr) {
        return Error::MEMORY;
    }

    token->type         = Token::Type::DONE;
    token->length       = 0U;
    tokens.currentToken = tokens.data.data();

    return countCountainersElements(tokens, counters) ? Error::NONE : Error::MEMORY;
}

bool Lexer::readString(Token &token) noexcept {
    const unsigned position = m_position + 1U;
    bool escaping = false;
    
    unsigned i;
    for(i = 0U; position + i < m_length; i++) {
        const char c = m_data[position + i];
        
        if(c == '\\' && !escaping) {
            escaping = true;
        } else if(escaping) {
            escaping = false;
        } else if(c == '"') {
            token.type   = Token::Type::STRING;
            token.length = i + 2U;
            return true;
        }
    }
    
    token.type = Token::Type::INVALID;
    token.length = i + 1U;
    return false;
}

bool Lexer::readNumber(Token &token) noexcept {
    const char *data   = m_data;
    unsigned    length = m_length;

    unsigned position, i;
    bool success   = true;
    bool read_dot  = false;
    bool read_e    = false;
    bool read_sign = false;

    if(m_data[m_position] == '-') {
        position     = m_position + 1U;
        token.length = 1U;
    } else {
        position     = m_position;
        token.length = 0U;
    }

    data       += position;
    length     -= position;
    token.type  = Token::Type::INT;

    //0 as the first character is only allowed if it's followed by a dot or by a non-digit character
    if(data[0] == '0' && length > 1U && Util::isDigit(data[1])) {
        success    = false;
        token.type = Token::Type::INVALID;

        for(i = 0U; i < length; i++) {
            const char c = data[i];
            if(Util::isWhitespace(c) || Util::isDelimiter(c)) {
                break;
            }
        }
    } 
    
    else for(i = 0U; i < length; i++) {
        const char c = data[i];
        if(Util::isWhitespace(c) || Util::isDelimiter(c)) {
            break;
        }
        
        switch(c) {
        case '.': {
            token.type = Token::Type::FLOAT;
            if(!read_dot) {
                read_dot = true;
            } else {
                token.type = Token::Type::INVALID;
                success    = false;
            }
            break;
        }

        case 'E': {
        case 'e':
            if(!read_e) {
                read_e     = true;
                token.type = Token::Type::SCIENTIFIC_INT;
            } else {
                token.type = Token::Type::INVALID;
                success    = false;
            }
            break;
        }

        case '+':
        case '-': {
            if(read_e && !read_sign) {
                read_sign = true;
            } else {
                token.type = Token::Type::INVALID;
                success    = false;                
            }
            break;
        }

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            continue;

        default: {
            token.type = Token::Type::INVALID;
            success    = false;
        }
        }
    }

    token.length += i;

    return success;
}

bool Lexer::readKeyword(Token &token) noexcept {   
    static const struct {
        std::string value;
        Token::Type type;
    } keywords[] = {
        { "null" , Token::Type::NUL},
        { "true" , Token::Type::BOOL},
        { "false", Token::Type::BOOL}
    };

    for(const auto &keyword : keywords) {
        if(m_position + static_cast<unsigned>(keyword.value.size()) - 1U >= m_length) {
            continue;
        }

        if(std::strncmp(m_data + m_position, keyword.value.c_str(), keyword.value.size()) != 0) {
            continue;
        }
        
        if(static_cast<size_t>(m_position) + keyword.value.size() >= m_length) {
            token.type   = keyword.type;
            token.length = static_cast<unsigned>(keyword.value.size());
            return true;
        }

        const char nextChar = m_data[m_position + keyword.value.size()];
        if(Util::isWhitespace(nextChar) || Util::isDelimiter(nextChar)) {
            token.type   = keyword.type;
            token.length = static_cast<unsigned>(keyword.value.size());
            return true;
        }
        
    }

    return false;
}

void Lexer::readInvalidToken(Token &token) noexcept {
    unsigned i;
    for(i = m_position; i < m_length; i++) {
        const char c = m_data[i];
        if(Util::isWhitespace(c) || Util::isDelimiter(c)) {
            i++;
            break;
        };
    }
    
    token.type   = Token::Type::INVALID;
    token.length = i - m_position - 1U;
}

}