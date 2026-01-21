#pragma once

namespace CPPJSON {

enum class Error {
    NONE,
    TOKEN,
    STRING,
    FLOAT64,
    INT64,
    UINT64,
    OBJECT,
    OBJECT_VALUE,
    OBJECT_KEY,
    ARRAY,
    ARRAY_VALUE,
    MISSING_COLON,
    MISSING_COMMA_OR_RCURLY,
    MISSING_COMMA_OR_RBRACKET,
    FILE,
    MEMORY,
    TOO_LARGE
};

const char *getErrorString(Error error) noexcept;

}

