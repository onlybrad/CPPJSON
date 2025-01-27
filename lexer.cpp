#include <cstring>
#include <iostream>
#include "lexer.hpp"
#include "util.hpp"
#include "parser.hpp"

namespace CJSON {

Lexer::Lexer(const std::string &data): m_data(data) {}

void Lexer::skipWhitespace() {
    while(m_position < m_data.size() && isWhitespace(m_data[m_position])) {
        m_position++;
    }
}

bool Lexer::readString(Token &token) {
    bool escaping = false;
    unsigned int position = m_position + 1;

    unsigned int i;
    for(i = 0U; position + i < m_data.size(); i++) {
        const char c = m_data[position + i];
        
        if(c == '\\' && !escaping) {
            escaping = true;
        } else if(escaping) {
            escaping = false;
        } else if(c == '"') {
            token.type = TokenType::STRING;
            token.value.size = i + 2U;
            return true;
        }
    }

    token.type = TokenType::INVALID;
    token.value.size = i + 1U;
    return false;
}

bool Lexer::readNumber(Token &token) {
    unsigned int position, i, size;
    size = (unsigned int)m_data.size(); 
    const char *data = m_data.c_str();
    bool success = true;
    bool read_dot = false;
    bool read_e = false;
    bool read_sign = false;

    if(data[m_position] == '-') {
        position = m_position + 1U;
        token.value.size = 1U;
    } else {
        position = m_position;
        token.value.size = 0U;
    }

    data += position;
    size -= position;
    token.type = TokenType::INT;

    //0 as the first character is only allowed if it's followed by a dot or by an non-digit character
    if(data[0] == '0' && size > 1 && data[1] != '.' && isDigit(data[1])) {
        success = false;
        token.type = TokenType::INVALID;

        for(i = 0U; i < size; i++) {
            const char c = data[i];

            if(isWhitespace(c) || isDelimiter(c)) {
                break;
            }
        }
    } 
    
    else for(i = 0U; i < size; i++) {
        const char c = data[i];

        if(isWhitespace(c) || isDelimiter(c)) {
            break;
        }
        
        switch(c) {
        case '.': {
            token.type = TokenType::FLOAT;
            if(!read_dot) {
                read_dot = true;
            } else {
                token.type = TokenType::INVALID;
                success = false;
            }
            break;
        }

        case 'e':
        case 'E': {
            if(!read_e) {
                read_e = true;
                token.type = TokenType::SCIENTIFIC_INT;
            } else {
                token.type = TokenType::INVALID;
                success = false;
            }
            break;
        }

        case '+':
        case '-': {
            if(read_e && !read_sign) {
                read_sign = true;
            } else {
                token.type = TokenType::INVALID;
                success = false;                
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
            token.type = TokenType::INVALID;
            success = false;
        }
        }
    }

    token.value.size += i;

    return success;
}

bool Lexer::isKeyword(const StringView keyword) {
    if(m_position + keyword.size - 1 < m_data.size() 
    && std::strncmp(m_data.c_str() + m_position, keyword.data, keyword.size) == 0
    ){
        if(m_position + keyword.size >= m_data.size()) {
            return true;
        }

        const char next_char = m_data[m_position + keyword.size];

        return isWhitespace(next_char) || isDelimiter(next_char);
    }

    return false;
}


bool Lexer::readKeyword(Token &token) {
    static const StringView null_string(STATIC_STRING("null"));
    static const StringView true_string(STATIC_STRING("true"));
    static const StringView false_string(STATIC_STRING("false"));

    if(isKeyword(null_string)) {
        token.type = TokenType::NULL_T;
        token.value.size = null_string.size;
        return true;
    }
    
    if(isKeyword(true_string)) {
        token.type = TokenType::BOOL;
        token.value.size = true_string.size;
        return true;
    }
    
    if(isKeyword(false_string)) {
        token.type = TokenType::BOOL;
        token.value.size = false_string.size;
        return true;
    }

    return false;
}

void Lexer::readInvalidToken(Token &token) {
    const unsigned int length = (unsigned int)m_data.size();

    unsigned int i;
    for(i = m_position; i < length; i++) {
        if(isWhitespace(m_data[i]) || isDelimiter(m_data[i])) {
            i++;
            break;
        };
    }
    
    token.type = TokenType::INVALID;
    token.value.size = i - m_position - 1;
}

bool Lexer::tokenize(Token &token) {
    skipWhitespace();

    if(m_position == m_data.size()) {
        token.type = TokenType::NULL_T;
        token.value.data = nullptr;
        token.value.size = 0;
        return false;
    }

    token.value.data = m_data.c_str() + m_position;

    switch(*token.value) {
    case '{':
        token.value.size = 1U;
        token.type = TokenType::LCURLY;
        break;
    case '}':
        token.value.size = 1U;
        token.type = TokenType::RCURLY;
        break;
    case '[':
        token.value.size = 1U;
        token.type = TokenType::LBRACKET;
        break;
    case ']':
        token.value.size = 1U;
        token.type = TokenType::RBRACKET;
        break;
    case ':':
        token.value.size = 1U;
        token.type = TokenType::COLON;
        break;
    case ',':
        token.value.size = 1U;
        token.type = TokenType::COMMA;
        break;
    case '"': {
        if(!readString(token)) {
            return false;
        }
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
        if(!readNumber(token)) {
            return false;
        }
        break;
    }
    default: {
        if(!readKeyword(token)) {
            readInvalidToken(token);
            return false;
        }
        break;
    }    
    }

    m_position += token.value.size;
    
    return true;
}

}
