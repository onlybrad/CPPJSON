#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <memory>
#include "token.hpp"

namespace CJSON {

class JSON_Node;

using Array = std::vector<JSON_Node>;
using Object = std::unordered_map<std::string, JSON_Node>;

enum class Type {
    ERROR,
    STRING,
    FLOAT64,
    INT64,
    UINT64,
    ARRAY,
    OBJECT,
    NULL_T,
    BOOL
};

enum class Error {
    TOKEN_ERROR,
    STRING_FAILED_TO_PARSE,
    FLOAT64_FAILED_TO_PARSE,
    INT64_FAILED_TO_PARSE,
    UINT64_FAILED_TO_PARSE,
    OBJECT_FAILED_TO_PARSE,
    OBJECT_INVALID_VALUE,
    OBJECT_INVALID_KEY,
    OBJECT_MISSING_COLON,
    OBJECT_MISSING_COMMA_OR_RCURLY,
    ARRAY_FAILED_TO_PARSE,
    ARRAY_MISSING_COMMA_OR_RBRACKET,
    ARRAY_INVALID_VALUE,
    FAILED_TO_OPEN_FILE
};

enum class ObjectParsingError {
    KeyError,
    ColonError,
    ValueError,
    CommaError,
    IncompleteError,
};

enum class ArrayParsingError {
    ValueError,
    CommaError,
    IncompleteError
};

union Data {
    std::string string;
    double      float64;
    int64_t     int64;
    uint64_t    uint64;
    Array       array;
    Object      object;
    void        *null;
    bool        boolean;
    Error       error;

    Data();
    ~Data(); 
};

class JSON_Node {
protected:
    Data m_value;
    Type m_type = Type::NULL_T;

    static std::string parseUtf8String(const Token &token, bool &success);
    bool parseString(Tokens &tokens);
    bool parseNumber(Tokens &tokens);
    void parseBool(Tokens &tokens);
    void parseNull(Tokens &tokens);
    bool parseArray(Tokens &tokens);
    bool parseObject(Tokens &tokens);
    bool parseTokens(Tokens &tokens);

public:
    JSON_Node() = default;
    JSON_Node(JSON_Node &&other);
    ~JSON_Node();
    Array &makeArray();
    Object &makeObject();
    std::string &makeString();
};


class JSON : public JSON_Node {
    Tokens m_tokens;
    JSON();
public:
    static std::unique_ptr<JSON> parse(const std::string &data);
};

}
