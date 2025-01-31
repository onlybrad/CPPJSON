#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <memory>
#include "token.hpp"

namespace CJSON {

class Node;
class Array;
class Object;

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

class Array {
    friend class Object;
    friend class Node;

    std::vector<Node> m_nodes;

public:
    Node *get(const unsigned int index, bool &success);
    Array *getArray(const unsigned int index, bool &success);
    Object *getObject(const unsigned int index, bool &success);
    int64_t getInt64(const unsigned int index, bool &success);
    uint64_t getUint64(const unsigned int index, bool &success);
    double getFloat64(const unsigned int index, bool &success);
    bool getBool(const unsigned int index, bool &success);
    std::string *getString(const unsigned int index, bool &success);
    std::nullptr_t getNull(const unsigned int index, bool &success);

    void set(const unsigned int index, Node &&node);
    void set(const unsigned int index, Array &&value);
    void set(const unsigned int index, Object &&value);
    void set(const unsigned int index, const int64_t value);
    void set(const unsigned int index, const uint64_t value);
    void set(const unsigned int index, const double value);
    void set(const unsigned int index, const bool value);
    void set(const unsigned int index, std::string &&value);
    void set(const unsigned int index, std::nullptr_t null);

    void push_back(Array &&value);
    void push_back(Object &&value);
    void push_back(const int64_t value);
    void push_back(const uint64_t value);
    void push_back(const double value);
    void push_back(const bool value);
    void push_back(std::string &&value);
    void push_back(std::nullptr_t null);

    unsigned int size();

    void destroy();

    Array();
    ~Array() = default;
    Array(Array &&other);
};

class Object {
    friend class Array;
    friend class Node;

    std::unordered_map<std::string, Node> m_nodes;

public:
    Node *operator[](std::string &key);
    Node *get(std::string &key, bool &success);
    Array *getArray(std::string &key, bool &success);
    Object *getObject(std::string &key, bool &success);
    int64_t getInt64(std::string &key, bool &success);
    uint64_t getUint64(std::string &key, bool &success);
    double getFloat64(std::string &key, bool &success);
    bool getBool(std::string &key, bool &success);
    std::string *getString(std::string &key, bool &success);
    std::nullptr_t getNull(std::string &key, bool &success);

    void set(std::string &key, Node &&value);
    void set(std::string &key, Array &&value);
    void set(std::string &key, Object &&value);
    void set(std::string &key, const int64_t value);
    void set(std::string &key, const uint64_t value);
    void set(std::string &key, const double value);
    void set(std::string &key, const bool value);
    void set(std::string &key, std::string &&value);
    void set(std::string &key, std::nullptr_t null);

    void set(std::string &&key, Node &&value);
    void set(std::string &&key, Array &&value);
    void set(std::string &&key, Object &&value);
    void set(std::string &&key, const int64_t value);
    void set(std::string &&key, const uint64_t value);
    void set(std::string &&key, const double value);
    void set(std::string &&key, const bool value);
    void set(std::string &&key, std::string &&value);
    void set(std::string &&key, std::nullptr_t null);

    void destroy();

    Object();
    ~Object() = default;
    Object(Object &&other);
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

class Node {
protected:
    friend class Array;
    friend class Object;
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
    Node() = default;
    Node(Node &&other);
    ~Node();

    Node &operator=(Node &&other);

    Array &makeArray();
    Object &makeObject();
    std::string &makeString();

    Type type() const;
};


class JSON : public Node {
    Tokens m_tokens;
    JSON();
public:
    static std::unique_ptr<JSON> parse(const std::string &data);
};

}
