#pragma once

#include <vector>
#include "stringview.hpp"

namespace CJSON {

enum class TokenType {
    LCURLY,
    RCURLY,
    LBRACKET,
    RBRACKET,
    COLON,
    COMMA,
    STRING,
    INT,
    FLOAT,
    SCIENTIFIC_INT,
    BOOL,
    NULL_T,
    INVALID
};

struct Token {
    TokenType type;
    StringView value;

    Token();
    Token(Token &&other);
    ~Token();
};

struct Tokens {
    std::vector<Token> data;
    unsigned int index = 0;

    Tokens();
    Token *nextToken();
};

}
