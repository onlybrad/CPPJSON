#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <memory>
#include "token.hpp"

namespace CJSON {

class JSON;
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

class Query {
    JSON *m_node;
    
public:
    Query() = delete;
    Query(JSON *node);

    Query operator[](const unsigned int index);
    Query operator[](const std::string &key);
    
    JSON *get();
    Array *getArray(bool &success);
    Object *getObject(bool &success);
    int64_t getInt64(bool &success);
    uint64_t getUint64(bool &success);
    double getFloat64(bool &success);
    bool getBool(bool &success);
    std::string *getString(bool &success);
    std::nullptr_t getNull(bool &success);
};

class Array {
    friend class Object;
    friend class JSON;

    std::vector<JSON> m_nodes;

public:
    Array &operator=(Array &&other);
    JSON *operator[](const unsigned int index);

    JSON *get(const unsigned int index, bool &success);
    Array *getArray(const unsigned int index, bool &success);
    Object *getObject(const unsigned int index, bool &success);
    int64_t getInt64(const unsigned int index, bool &success);
    uint64_t getUint64(const unsigned int index, bool &success);
    double getFloat64(const unsigned int index, bool &success);
    bool getBool(const unsigned int index, bool &success);
    std::string *getString(const unsigned int index, bool &success);
    std::nullptr_t getNull(const unsigned int index, bool &success);

    void set(const unsigned int index, JSON &&node);
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
    Array(Array &&other);
};

class Object {
    friend class Array;
    friend class JSON;

    std::unordered_map<std::string, JSON> m_nodes;

public:
    Object &operator=(Object &&other);
    JSON *operator[](const std::string &key);

    JSON *get(const std::string &key, bool &success);
    Array *getArray(const std::string &key, bool &success);
    Object *getObject(const std::string &key, bool &success);
    int64_t getInt64(const std::string &key, bool &success);
    uint64_t getUint64(const std::string &key, bool &success);
    double getFloat64(const std::string &key, bool &success);
    bool getBool(const std::string &key, bool &success);
    std::string *getString(const std::string &key, bool &success);
    std::nullptr_t getNull(const std::string &key, bool &success);

    void set(const std::string &key, JSON &&value);
    void set(const std::string &key, Array &&value);
    void set(const std::string &key, Object &&value);
    void set(const std::string &key, const int64_t value);
    void set(const std::string &key, const uint64_t value);
    void set(const std::string &key, const double value);
    void set(const std::string &key, const bool value);
    void set(const std::string &key, std::string &&value);
    void set(const std::string &key, std::nullptr_t null);

    void set(std::string &&key, JSON &&value);
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
    Object(Object &&other);
};

union Data {
    std::string    string;
    double         float64;
    int64_t        int64;
    uint64_t       uint64;
    Array          array;
    Object         object;
    std::nullptr_t null;
    bool           boolean;
    Error          error;

    Data();
    ~Data(); 
    Data &operator=(Data &&other);
};

class JSON {
protected:
    friend class Array;
    friend class Object;
    Data m_data;
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
    static std::unique_ptr<JSON> parse(const std::string &data);
    
    JSON() = default;
    JSON(JSON &&other);
    ~JSON();

    JSON &operator=(JSON &&other);
    Query operator[](const unsigned int index);
    Query operator[](const std::string &key);

    Array &makeArray();
    Object &makeObject();
    std::string &makeString();
    void set(const std::string &str);
    void set(std::string &&str);
    void set(const int64_t value);
    void set(const uint64_t value);
    void set(const double value);
    void set(const bool value);
    void set(std::nullptr_t null);
    void destroy();
    Type type() const;
    Data &value();
};

std::unique_ptr<JSON> parse(const std::string &data);

}
