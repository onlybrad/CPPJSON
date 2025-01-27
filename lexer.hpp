#pragma once

#include <string>
#include "stringview.hpp"
#include "token.hpp"

namespace CJSON {

class Lexer {
    const std::string &m_data;
    unsigned int m_position = 0;

    void skipWhitespace();
    bool readString(Token &token);
    bool readNumber(Token &token);
    bool isKeyword(const StringView keyword);
    bool readKeyword(Token &token);
    void readInvalidToken(Token &token);

public:
    Lexer(const std::string &data);
    bool tokenize(Token &token);

};

}
