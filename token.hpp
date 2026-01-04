#pragma once

#include <stdio.h>

namespace CPPJSON {

struct Tokens;

struct Token {
    enum class Type {
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
        NUL,
        INVALID,
        DONE
    };

    const char *value;
    unsigned    length;
    Type        type;
};

}