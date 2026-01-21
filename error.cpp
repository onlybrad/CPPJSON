#include "error.hpp"

namespace CPPJSON {

const char *getErrorString(Error error) noexcept {
    switch(error) {
    case Error::NONE:
        return "No Error.";
    case Error::TOKEN:
        return "Token error.";
    case Error::STRING:
        return "String failed to parse.";
    case Error::FLOAT64:
        return "Float64 failed to parse.";
    case Error::INT64:
        return "Int64 failed to parse.";
    case Error::UINT64:
        return "Uint64 failed to parse.";
    case Error::OBJECT:
        return "Object failed to parse.";
    case Error::OBJECT_KEY:
        return "Object invalid key.";
    case Error::OBJECT_VALUE:
        return "Object invalid value.";
    case Error::MISSING_COLON:
        return "Object missing colon.";
    case Error::MISSING_COMMA_OR_RCURLY:
        return "Missing comma or right curly bracket.";
    case Error::MISSING_COMMA_OR_RBRACKET:
        return "Missing comma or right bracket.";
    case Error::ARRAY:
        return "Array failed to parse.";
    case Error::ARRAY_VALUE:
        return "Array invalid value.";
    case Error::FILE:
        return "Failed to open file.";
    case Error::MEMORY:
        return "Failed to allocate memory.";
    case Error::TOO_LARGE:
        return "File/string/number too large. Maximum supported is UINT_MAX.";
    }

    return nullptr;
}

}