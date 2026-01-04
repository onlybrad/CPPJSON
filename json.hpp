#pragma once

#include <string>
#include <cstdint>

#include "object.hpp"
#include "array.hpp"
#include "string.hpp"

namespace CPPJSON {

class Parser;

class QueryBuilder {
    friend class JSON;

    JSON *m_json;

public:
    QueryBuilder(JSON *json) noexcept;

    QueryBuilder& operator[](unsigned)           noexcept;
    QueryBuilder& operator[](const std::string&) noexcept;
    QueryBuilder& operator[](const String&)      noexcept;
    QueryBuilder& operator[](const char*)        noexcept;
    QueryBuilder& operator[](std::nullptr_t)     noexcept = delete;

    JSON          *get      ()                    noexcept;
    String        *asString (bool &success)       noexcept;
    double         asFloat64(bool &success) const noexcept;
    int64_t        asInt64  (bool &success) const noexcept;
    uint64_t       asUint64 (bool &success) const noexcept;
    Object        *asObject (bool &success)       noexcept;
    Array         *asArray  (bool &success)       noexcept;
    std::nullptr_t asNull   (bool &success) const noexcept;
    bool           asBool   (bool &success) const noexcept;
    String        *asString ()                    noexcept;
    double         asFloat64()              const noexcept;
    int64_t        asInt64  ()              const noexcept;
    uint64_t       asUint64 ()              const noexcept;
    Object        *asObject ()                    noexcept;
    Array         *asArray  ()                    noexcept;
    std::nullptr_t asNull   ()              const noexcept;
    bool           asBool   ()              const noexcept;
};

class JSON {
    friend class Parser;
    friend class QueryBuilder;

public:
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
        FILE,
        MEMORY,
        TOO_LARGE
    };

    union Value {
        String          string;
        double          float64;
        std::int64_t    int64;
        std::uint64_t   uint64;
        Object          object;
        Array           array;
        std::nullptr_t *null = nullptr;
        bool            boolean;
        JSON::Error     error;

        Value(double)                   noexcept;
        Value(std::int64_t)             noexcept;
        Value(std::uint64_t)            noexcept;
        Value(std::nullptr_t = nullptr) noexcept;
        Value(bool)                     noexcept;
        Value(JSON::Error)              noexcept;
        ~Value() noexcept;

        Value &operator=(const String&);
        Value &operator=(String&&)       noexcept;
        Value &operator=(double)         noexcept;
        Value &operator=(std::int64_t)   noexcept;
        Value &operator=(std::uint64_t)  noexcept;
        Value &operator=(const Object&);
        Value &operator=(Object&&)       noexcept;
        Value &operator=(const Array&);
        Value &operator=(Array&&)        noexcept;
        Value &operator=(std::nullptr_t) noexcept;
        Value &operator=(bool)           noexcept;
        Value &operator=(JSON::Error)    noexcept;
    };

    enum class Type {
        ERROR,
        STRING,
        FLOAT64,
        INT64,
        UINT64,
        ARRAY,
        OBJECT,
        NUL,
        BOOL
    };
                     
    JSON(double)                   noexcept;                  
    JSON(std::int64_t)             noexcept;                  
    JSON(std::uint64_t)            noexcept; 
    JSON(int)                      noexcept;
    JSON(unsigned)                 noexcept;
    JSON(bool)                     noexcept;
    JSON(std::nullptr_t = nullptr) noexcept;                     
    JSON(const JSON&);
    JSON(JSON&&)                   noexcept;
    ~JSON()                        noexcept;
    JSON &operator=(const JSON&);
    JSON &operator=(JSON&&)        noexcept;

    Type  getType () const noexcept;
    Error getError() const noexcept;

    QueryBuilder operator[](unsigned)           noexcept;
    QueryBuilder operator[](const std::string&) noexcept;
    QueryBuilder operator[](const String&)      noexcept;
    QueryBuilder operator[](const char*)        noexcept;
    QueryBuilder operator[](std::nullptr_t)     noexcept = delete;

    String        *asString ()                    noexcept;
    double         asFloat64()                    noexcept;
    int64_t        asInt64  ()              const noexcept;
    uint64_t       asUint64 ()              const noexcept;
    Object        *asObject ()                    noexcept;
    Array         *asArray  ()                    noexcept;
    std::nullptr_t asNull   ()              const noexcept;
    bool           asBool   ()              const noexcept;
    String        *asString (bool &success)       noexcept;
    double         asFloat64(bool &success) const noexcept;
    int64_t        asInt64  (bool &success) const noexcept;
    uint64_t       asUint64 (bool &success) const noexcept;
    Object        *asObject (bool &success)       noexcept;
    Array         *asArray  (bool &success)       noexcept;
    std::nullptr_t asNull   (bool &success) const noexcept;
    bool           asBool   (bool &success) const noexcept;

    Object *makeObject(const Object::Allocator&) noexcept;
    Array  *makeArray(const Array::Allocator&)   noexcept;
    String *makeString(const String::Allocator&) noexcept;

    JSON &set(const std::string&, const String::Allocator&);
    JSON &set(const char*, const String::Allocator&);
    JSON &set(const String&);
    JSON &set(JSON&&);
    JSON &set(String&&);     
    JSON &set(double)                   noexcept;
    JSON &set(int64_t)                  noexcept;
    JSON &set(uint64_t)                 noexcept;
    JSON &set(int)                      noexcept;
    JSON &set(unsigned)                 noexcept;
    JSON &set(Object&&);
    JSON &set(Array&&);
    JSON &set(std::nullptr_t = nullptr) noexcept;
    JSON &set(bool)                     noexcept;

    JSON &operator=(const String&);     
    JSON &operator=(String&&);
    JSON &operator=(double)          noexcept;
    JSON &operator=(int64_t)         noexcept;
    JSON &operator=(uint64_t)        noexcept;
    JSON &operator=(int)             noexcept;
    JSON &operator=(unsigned)        noexcept;
    JSON &operator=(Object&&);
    JSON &operator=(Array&&);
    JSON &operator=(std::nullptr_t)  noexcept;
    JSON &operator=(bool)            noexcept;

private:
    Type  m_type = Type::NUL;
    Value m_value;
    
    JSON &set(Error)       noexcept;
    void destructor()      noexcept;
    void copy(const JSON&);
};

}