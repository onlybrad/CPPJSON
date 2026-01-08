#pragma once

#include <cassert>
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

    template<typename T>
    using AsCallbackRef = void(*)(T&);

    template<typename T>
    using AsCallback = void(*)(T);
    
    typedef void(*FailureCallback)();

    template<typename T, T*(JSON::*method)(bool&)>
    T *asPtr(bool &success) noexcept;

    template<typename T, T(JSON::*method)(bool&) const>
    T as(bool &success) const noexcept;

    template<typename T, T*(JSON::*method)(bool&)>
    bool as(AsCallbackRef<T> callback, FailureCallback failureCallback) noexcept;

    template<typename T, T(JSON::*method)(bool&) const>
    bool as(AsCallback<T> callback, FailureCallback failureCallback) const noexcept;

    template<typename T, T*(JSON::*method)(bool&)>
    bool as(AsCallbackRef<T> callback) noexcept;

    template<typename T, T(JSON::*method)(bool&) const>
    bool as(AsCallback<T> callback) const noexcept;

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

    String        *asString ()       noexcept;
    double         asFloat64() const noexcept;
    int64_t        asInt64  () const noexcept;
    uint64_t       asUint64 () const noexcept;
    Object        *asObject ()       noexcept;
    Array         *asArray  ()       noexcept;
    std::nullptr_t asNull   () const noexcept;
    bool           asBool   () const noexcept;

    bool asString (AsCallbackRef<String>,      FailureCallback)       noexcept;
    bool asFloat64(AsCallback<double>,         FailureCallback) const noexcept;
    bool asInt64  (AsCallback<std::int64_t>,   FailureCallback) const noexcept;
    bool asUint64 (AsCallback<std::uint64_t>,  FailureCallback) const noexcept;
    bool asObject (AsCallbackRef<Object>,      FailureCallback)       noexcept;
    bool asArray  (AsCallbackRef<Array>,       FailureCallback)       noexcept;
    bool asNull   (AsCallback<std::nullptr_t>, FailureCallback) const noexcept;
    bool asBool   (AsCallback<bool>,           FailureCallback) const noexcept;

    bool asString (AsCallbackRef<String>)            noexcept;
    bool asFloat64(AsCallback<double>)         const noexcept;
    bool asInt64  (AsCallback<std::int64_t>)   const noexcept;
    bool asUint64 (AsCallback<std::uint64_t>)  const noexcept;
    bool asObject (AsCallbackRef<Object>)            noexcept;
    bool asArray  (AsCallbackRef<Array>)             noexcept;
    bool asNull   (AsCallback<std::nullptr_t>) const noexcept;
    bool asBool   (AsCallback<bool>)           const noexcept;

    bool asString (AsCallbackRef<String>,      std::nullptr_t)       noexcept = delete;
    bool asFloat64(AsCallback<double>,         std::nullptr_t) const noexcept = delete;
    bool asInt64  (AsCallback<std::int64_t>,   std::nullptr_t) const noexcept = delete;
    bool asUint64 (AsCallback<std::uint64_t>,  std::nullptr_t) const noexcept = delete;
    bool asObject (AsCallbackRef<Object>,      std::nullptr_t)       noexcept = delete;
    bool asArray  (AsCallbackRef<Array>,       std::nullptr_t)       noexcept = delete;
    bool asNull   (AsCallback<std::nullptr_t>, std::nullptr_t) const noexcept = delete;
    bool asBool   (AsCallback<bool>,           std::nullptr_t) const noexcept = delete;

    bool asString (std::nullptr_t)       noexcept = delete;
    bool asFloat64(std::nullptr_t) const noexcept = delete;
    bool asInt64  (std::nullptr_t) const noexcept = delete;
    bool asUint64 (std::nullptr_t) const noexcept = delete;
    bool asObject (std::nullptr_t)       noexcept = delete;
    bool asArray  (std::nullptr_t)       noexcept = delete;
    bool asNull   (std::nullptr_t) const noexcept = delete;
    bool asBool   (std::nullptr_t) const noexcept = delete;
};

class JSON {
    friend class Parser;
    friend class QueryBuilder;
    template<typename T>
    using AsCallbackRef = void(*)(T&);

    template<typename T>
    using AsCallback = void(*)(T);
    
    typedef void(*FailureCallback)();

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
        std::nullptr_t  null = nullptr;
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

    String        *asString ()       noexcept;
    double         asFloat64()       noexcept;
    int64_t        asInt64  () const noexcept;
    uint64_t       asUint64 () const noexcept;
    Object        *asObject ()       noexcept;
    Array         *asArray  ()       noexcept;
    std::nullptr_t asNull   () const noexcept;
    bool           asBool   () const noexcept;

    String        *asString (bool &success)       noexcept;
    double         asFloat64(bool &success) const noexcept;
    int64_t        asInt64  (bool &success) const noexcept;
    uint64_t       asUint64 (bool &success) const noexcept;
    Object        *asObject (bool &success)       noexcept;
    Array         *asArray  (bool &success)       noexcept;
    std::nullptr_t asNull   (bool &success) const noexcept;
    bool           asBool   (bool &success) const noexcept;

    bool asString (AsCallbackRef<String>,      FailureCallback)       noexcept;
    bool asFloat64(AsCallback<double>,         FailureCallback) const noexcept;
    bool asInt64  (AsCallback<std::int64_t>,   FailureCallback) const noexcept;
    bool asUint64 (AsCallback<std::uint64_t>,  FailureCallback) const noexcept;
    bool asObject (AsCallbackRef<Object>,      FailureCallback)       noexcept;
    bool asArray  (AsCallbackRef<Array>,       FailureCallback)       noexcept;
    bool asNull   (AsCallback<std::nullptr_t>, FailureCallback) const noexcept;
    bool asBool   (AsCallback<bool>,           FailureCallback) const noexcept;

    bool asString (AsCallbackRef<String>)            noexcept;
    bool asFloat64(AsCallback<double>)         const noexcept;
    bool asInt64  (AsCallback<std::int64_t>)   const noexcept;
    bool asUint64 (AsCallback<std::uint64_t>)  const noexcept;
    bool asObject (AsCallbackRef<Object>)            noexcept;
    bool asArray  (AsCallbackRef<Array>)             noexcept;
    bool asNull   (AsCallback<std::nullptr_t>) const noexcept;
    bool asBool   (AsCallback<bool>)           const noexcept;

    bool asString (AsCallbackRef<String>,      std::nullptr_t)       noexcept = delete;
    bool asFloat64(AsCallback<double>,         std::nullptr_t) const noexcept = delete;
    bool asInt64  (AsCallback<std::int64_t>,   std::nullptr_t) const noexcept = delete;
    bool asUint64 (AsCallback<std::uint64_t>,  std::nullptr_t) const noexcept = delete;
    bool asObject (AsCallbackRef<Object>,      std::nullptr_t)       noexcept = delete;
    bool asArray  (AsCallbackRef<Array>,       std::nullptr_t)       noexcept = delete;
    bool asNull   (AsCallback<std::nullptr_t>, std::nullptr_t) const noexcept = delete;
    bool asBool   (AsCallback<bool>,           std::nullptr_t) const noexcept = delete;

    bool asString (std::nullptr_t)       noexcept = delete;
    bool asFloat64(std::nullptr_t) const noexcept = delete;
    bool asInt64  (std::nullptr_t) const noexcept = delete;
    bool asUint64 (std::nullptr_t) const noexcept = delete;
    bool asObject (std::nullptr_t)       noexcept = delete;
    bool asArray  (std::nullptr_t)       noexcept = delete;
    bool asNull   (std::nullptr_t) const noexcept = delete;
    bool asBool   (std::nullptr_t) const noexcept = delete;

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

    template<typename Tclass, Type type, Tclass Value::*member>
    Tclass *asPtr(bool &success) noexcept {
        if(m_type != type) {
            success = false;
            return nullptr;
        }
        success = true;
        return &(this->m_value.*member);
    }

    template<typename Tclass, Type type, Tclass Value::*member>
    Tclass as(bool &success) const noexcept {
        if(m_type != type) {
            success = false;
            return 0;
        }
        success = true;
        return this->m_value.*member;
    }

    template<typename Tclass, Type type, Tclass Value::*member>
    bool as(AsCallbackRef<Tclass> callback, FailureCallback failureCallback) noexcept {
        assert(callback != nullptr);
        assert(failureCallback != nullptr);

        bool success;
        Tclass *const value = asPtr<Tclass, type, member>(success);

        if(success) {
            callback(*value);
        } else {
            failureCallback();
        }
    
        return success;
    }

    template<typename Tclass, Type type, Tclass Value::*member>
    bool as(AsCallback<Tclass> callback, FailureCallback failureCallback) const noexcept {
        assert(callback != nullptr);
        assert(failureCallback != nullptr);

        bool success;
        const Tclass value = as<Tclass, type, member>(success);

        if(success) {
            callback(value);
            return true;
        } else {
            failureCallback();
        }

        return success;
    }

    template<typename Tclass, Type type, Tclass Value::*member>
    bool as(AsCallbackRef<Tclass> callback) noexcept {
        assert(callback != nullptr);

        bool success;
        Tclass *const value = asPtr<Tclass, type, member>(success);

        if(success) {
            callback(*value);
        }

        return success;
    }

    template<typename Tclass, Type type, Tclass Value::*member>
    bool as(AsCallback<Tclass> callback) const noexcept {
        assert(callback != nullptr);

        bool success;
        const Tclass value = as<Tclass, type, member>(success);

        if(success) {
            callback(value);
            return true;
        }
        return success;
    }
};

template<typename T, T*(JSON::*method)(bool&)>
T *QueryBuilder::asPtr(bool &success) noexcept {
    if(m_json == nullptr) {
        success = false;
        return nullptr;
    }

    return (m_json->*method)(success);
}

template<typename T, T(JSON::*method)(bool&) const>
T QueryBuilder::as(bool &success) const noexcept {
    if(m_json == nullptr) {
        success = false;
        return 0;
    }

    return (m_json->*method)(success);
}

template<typename T, T*(JSON::*method)(bool&)>
bool QueryBuilder::as(AsCallbackRef<T> callback, FailureCallback failureCallback) noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    bool success;
    T *const value = asPtr<T, method>(success);

    if(success) {
        callback(*value);
    } else {
        failureCallback();
    }
    
    return success;
}

template<typename T, T(JSON::*method)(bool&) const>
bool QueryBuilder::as(AsCallback<T> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    bool success;
    const T value = as<T, method>(success);

    if(success) {
        callback(value);
    } else {
        failureCallback();
    }
    
    return success;
}

template<typename T, T*(JSON::*method)(bool&)>
bool QueryBuilder::as(AsCallbackRef<T> callback) noexcept {
    assert(callback != nullptr);

    bool success;
    T *const value = asPtr<T, method>(success);

    if(success) {
        callback(*value);
    }

    return success;
}

template<typename T, T(JSON::*method)(bool&) const>
bool QueryBuilder::as(AsCallback<T> callback) const noexcept {
    assert(callback != nullptr);

    bool success;
    const T value = as<T, method>(success);

    if(success) {
        callback(value);
    }

    return success;
}

}