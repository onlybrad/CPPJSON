#pragma once

#include <cassert>
#include <string>
#include <cstdint>
#include <type_traits>

#include "error.hpp"
#include "object.hpp"
#include "array.hpp"
#include "string.hpp"
#include "result.hpp"

namespace CPPJSON {
class JSON;
class Parser;

template<bool IsConst>
class QueryBuilder {
    friend class JSON;
    template<typename T>
    using ConstAwarePtr = typename std::conditional<IsConst, const T*, T*>::type;
    template<typename T>
    using ConstAwareRef = typename std::conditional<IsConst, const T&, T&>::type;

    typedef ConstAwarePtr<JSON>   JSONPtr;
    typedef ConstAwareRef<JSON>   JSONRef;
    typedef ConstAwareRef<String> StringRef;
    typedef ConstAwareRef<Object> ObjectRef;
    typedef ConstAwareRef<Array>  ArrayRef; 

    JSONPtr m_json;

    template<typename T, Result<T&>(JSON::*method)()>
    Result<T&> asRef() noexcept;

    template<typename T, Result<T&>(JSON::*method)() const>
    Result<T&> asRef() noexcept;

    template<typename T, Result<T>(JSON::*method)() const>
    Result<T> asValue() noexcept;

public:
    QueryBuilder(JSONPtr json) noexcept;

    QueryBuilder& operator[](unsigned)           noexcept;
    QueryBuilder& operator[](const std::string&) noexcept;
    QueryBuilder& operator[](const String&)      noexcept;
    QueryBuilder& operator[](const char*)        noexcept;
    QueryBuilder& operator[](std::nullptr_t)     noexcept = delete;

    Result<JSONRef>        get      () noexcept;
    Result<StringRef>      asString () noexcept;
    Result<double>         asFloat64() noexcept;
    Result<std::int64_t>   asInt64  () noexcept;
    Result<std::uint64_t>  asUint64 () noexcept;
    Result<ObjectRef>      asObject () noexcept;
    Result<ArrayRef>       asArray  () noexcept;
    Result<std::nullptr_t> asNull   () noexcept;
    Result<bool>           asBool   () noexcept;

    JSONRef        unsafeGet      () noexcept;
    StringRef      unsafeAsString () noexcept;
    double         unsafeAsFloat64() noexcept;
    std::int64_t   unsafeAsInt64  () noexcept;
    std::uint64_t  unsafeAsUint64 () noexcept;
    ObjectRef      unsafeAsObject () noexcept;
    ArrayRef       unsafeAsArray  () noexcept;
    std::nullptr_t unsafeAsNull   () noexcept;
    bool           unsafeAsBool   () noexcept;
};
class JSON {
    
friend class Parser;

public:
    static const JSON INVALID_JSON;

    union Value {
        String         string;
        double         float64;
        std::int64_t   int64;
        std::uint64_t  uint64;
        Object         object;
        Array          array;
        std::nullptr_t null = nullptr;
        bool           boolean;

        Value(double)                   noexcept;
        Value(std::int64_t)             noexcept;
        Value(std::uint64_t)            noexcept;
        Value(std::nullptr_t = nullptr) noexcept;
        Value(bool)                     noexcept;
        ~Value()                        noexcept;

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
    };

    enum class Type {
        STRING,
        FLOAT64,
        INT64,
        UINT64,
        ARRAY,
        OBJECT,
        NUL,
        BOOL
    };
                     
    JSON(double)                         noexcept;                  
    JSON(std::int64_t)                   noexcept;                  
    JSON(std::uint64_t)                  noexcept; 
    JSON(int)                            noexcept;
    JSON(unsigned)                       noexcept;
    JSON(bool)                           noexcept;
    JSON(std::nullptr_t = nullptr)       noexcept;                     
    JSON(const JSON&);
    JSON(JSON&&)                         noexcept;
    ~JSON()                              noexcept;
    JSON &operator=(const JSON&);
    JSON &operator=(JSON&&)              noexcept;
    bool operator==(const JSON&)   const noexcept;
    bool operator!=(const JSON&)   const noexcept;

    Type getType() const noexcept;
    bool isNumber() const noexcept;
    bool isValid() const noexcept;

    QueryBuilder<false> operator[](unsigned)           noexcept;
    QueryBuilder<false> operator[](const std::string&) noexcept;
    QueryBuilder<false> operator[](const String&)      noexcept;
    QueryBuilder<false> operator[](const char*)        noexcept;
    QueryBuilder<false> operator[](std::nullptr_t)     noexcept = delete;

    QueryBuilder<true> operator[](unsigned)           const noexcept;
    QueryBuilder<true> operator[](const std::string&) const noexcept;
    QueryBuilder<true> operator[](const String&)      const noexcept;
    QueryBuilder<true> operator[](const char*)        const noexcept;
    QueryBuilder<true> operator[](std::nullptr_t)     const noexcept = delete;

    Result<String&> asString() noexcept;
    Result<Object&> asObject() noexcept;
    Result<Array&>  asArray () noexcept;

    Result<const String&>  asString () const noexcept;
    Result<double>         asFloat64() const noexcept;
    Result<std::int64_t>   asInt64  () const noexcept;
    Result<std::uint64_t>  asUint64 () const noexcept;
    Result<const Object&>  asObject () const noexcept;
    Result<const Array&>   asArray  () const noexcept;
    Result<std::nullptr_t> asNull   () const noexcept;
    Result<bool>           asBool   () const noexcept;

    String &unsafeAsString() noexcept;
    Object &unsafeAsObject() noexcept;
    Array  &unsafeAsArray () noexcept;
    
    const String  &unsafeAsString () const noexcept;
    double         unsafeAsFloat64() const noexcept;
    std::int64_t   unsafeAsInt64  () const noexcept;
    std::uint64_t  unsafeAsUint64 () const noexcept;
    const Object  &unsafeAsObject () const noexcept;
    const Array   &unsafeAsArray  () const noexcept;
    std::nullptr_t unsafeAsNull   () const noexcept;
    bool           unsafeAsBool   () const noexcept;

    Result<Object&> makeObject(const Object::Allocator&) noexcept;
    Result<Array&>  makeArray(const Array::Allocator&)   noexcept;
    Result<String&> makeString(const String::Allocator&) noexcept;

    JSON &set(const std::string&, const String::Allocator&);
    JSON &set(const char*, const String::Allocator&);
    JSON &set(const String&);
    JSON &set(JSON&&);
    JSON &set(String&&);
    JSON &set(double)                   noexcept;
    JSON &set(std::int64_t)             noexcept;
    JSON &set(std::uint64_t)            noexcept;
    JSON &set(int)                      noexcept;
    JSON &set(unsigned)                 noexcept;
    JSON &set(Object&&);
    JSON &set(Array&&);
    JSON &set(std::nullptr_t = nullptr) noexcept;
    JSON &set(bool)                     noexcept;

    JSON &operator=(const String&);     
    JSON &operator=(String&&);
    JSON &operator=(double)          noexcept;
    JSON &operator=(std::int64_t)    noexcept;
    JSON &operator=(std::uint64_t)   noexcept;
    JSON &operator=(int)             noexcept;
    JSON &operator=(unsigned)        noexcept;
    JSON &operator=(Object&&);
    JSON &operator=(Array&&);
    JSON &operator=(std::nullptr_t)  noexcept;
    JSON &operator=(bool)            noexcept;

    void        toString    (std::string&, unsigned indentation = 0U, unsigned level = 0U) const noexcept;
    std::string toString    (unsigned indentation = 0U)                                    const;
    unsigned    toStringSize(unsigned indentation = 0U, unsigned level = 1U)               const noexcept;
    bool        toFile      (const std::string&,        unsigned indentation = 0U)         const;
    bool        toFile      (const char*,               unsigned indentation = 0U)         const;
    
    static Result<std::string> format(const std::string&, unsigned indentation = 0U);
    static Result<std::string> format(const char*,        unsigned indentation = 0U);

private:
    Type  m_type = Type::NUL;
    Value m_value;
    
    void destructor() noexcept;
    void copy(const JSON&);

    template<typename TClass, Type type, TClass Value::*member>
    Result<TClass&> asRef() noexcept {
        return m_type != type
            ? Result<TClass&>::fromError(true)
            : Result<TClass&>::fromRef(m_value.*member);
    }

    template<typename TClass, Type type, TClass Value::*member>
    Result<const TClass&> asConstRef() const noexcept {
        return m_type != type
            ? Result<const TClass&>::fromError(true)
            : Result<const TClass&>::fromRef(m_value.*member);
    }

    template<typename TClass, Type type, TClass Value::*member>
    Result<TClass> asValue() const noexcept {
        return m_type != type
            ? Result<TClass>::fromError(true)
            : Result<TClass>::fromValue(m_value.*member);
    }
};

template<bool IsConst>
QueryBuilder<IsConst>::QueryBuilder(JSONPtr json) noexcept :
m_json(json)
{}

template<bool IsConst>
QueryBuilder<IsConst> &QueryBuilder<IsConst>::operator[](const unsigned index) noexcept {
    if(m_json == nullptr) {
        return *this;
    }

    if(m_json->getType() != JSON::Type::ARRAY) {
        m_json = nullptr;
        return *this;
    }

    unsafeAsArray().get(index)
        .onSuccess([this](JSONRef json) {
            m_json = &json;
        })
        .onError([this]() {
            m_json = nullptr;
        });

    return *this;
}

template <bool IsConst>
QueryBuilder<IsConst> &QueryBuilder<IsConst>::operator[](const std::string &key) noexcept {
    return (*this)[key.c_str()];
}

template<bool IsConst>
QueryBuilder<IsConst> &QueryBuilder<IsConst>::operator[](const String &key) noexcept {
    if(m_json == nullptr) {
        return *this;
    }

    if(m_json->getType() != JSON::Type::OBJECT) {
        m_json = nullptr;
        return *this;
    }

    unsafeAsObject().get(key)
        .onSuccess([this](JSONRef json) {
            m_json = &json;
        })
        .onError([this]() {
            m_json = nullptr;
        });

    return *this;
}

template <bool IsConst>
QueryBuilder<IsConst> &QueryBuilder<IsConst>::operator[](const char *const key) noexcept {
    assert(key != nullptr);

    String stringKey(Object::getKeyAllocator());
    stringKey = key;
    (*this)[stringKey];
    Object::getKeyAllocator().reset();

    return *this;
}

template<bool IsConst>
Result<typename QueryBuilder<IsConst>::JSONRef> QueryBuilder<IsConst>::get() noexcept {
    return m_json == nullptr
        ? Result<JSONRef>::fromError(true)
        : Result<JSONRef>::fromRef(*m_json);
}

template<bool IsConst>
Result<typename QueryBuilder<IsConst>::StringRef> QueryBuilder<IsConst>::asString() noexcept {
    return asRef<StringRef, &JSON::asString>();
}

template<bool IsConst>
Result<double> QueryBuilder<IsConst>::asFloat64() noexcept {
    return asValue<double, &JSON::asFloat64>();
}

template<bool IsConst>
Result<std::int64_t> QueryBuilder<IsConst>::asInt64() noexcept {
    return asValue<std::int64_t, &JSON::asInt64>();
}

template<bool IsConst>
Result<std::uint64_t> QueryBuilder<IsConst>::asUint64() noexcept {
    return asValue<std::uint64_t, &JSON::asUint64>();
}

template<bool IsConst>
Result<typename QueryBuilder<IsConst>::ObjectRef> QueryBuilder<IsConst>::asObject() noexcept { 
    return asRef<ObjectRef, &JSON::asObject>();
}

template<bool IsConst>
Result<typename QueryBuilder<IsConst>::ArrayRef> QueryBuilder<IsConst>::asArray() noexcept { 
    return asRef<ArrayRef, &JSON::asArray>();
}

template<bool IsConst>
Result<std::nullptr_t> QueryBuilder<IsConst>::asNull() noexcept {
    return asValue<std::nullptr_t, &JSON::asNull>();
}

template<bool IsConst>
Result<bool> QueryBuilder<IsConst>::asBool() noexcept {
    return asValue<bool, &JSON::asBool>();
}

template<bool IsConst>
template<typename T, Result<T&>(JSON::*method)()>
Result<T&> QueryBuilder<IsConst>::asRef() noexcept {
    return m_json == nullptr
        ? Result<T&>::fromError(true)
        : (m_json->*method)();
}

template<bool IsConst>
template<typename T, Result<T&>(JSON::*method)() const>
Result<T&> QueryBuilder<IsConst>::asRef() noexcept {
    return m_json == nullptr
        ? Result<T&>::fromError(true)
        : (m_json->*method)();
}

template<bool IsConst>
template<typename T, Result<T>(JSON::*method)() const>
Result<T> QueryBuilder<IsConst>::asValue() noexcept {
    return m_json == nullptr
        ? Result<T>::fromError(true)
        : (m_json->*method)();
}

template<bool IsConst>
typename QueryBuilder<IsConst>::JSONRef QueryBuilder<IsConst>::unsafeGet() noexcept {
    assert(m_json != nullptr);

    return *m_json;
}

template<bool IsConst>
typename QueryBuilder<IsConst>::StringRef QueryBuilder<IsConst>::unsafeAsString() noexcept {
    assert(m_json != nullptr);

    return m_json->unsafeAsString();
}

template<bool IsConst>
double QueryBuilder<IsConst>::unsafeAsFloat64() noexcept {
    assert(m_json != nullptr);

    return m_json->unsafeAsFloat64();
}

template<bool IsConst>
std::int64_t QueryBuilder<IsConst>::unsafeAsInt64() noexcept {
    assert(m_json != nullptr);

    return m_json->unsafeAsInt64();
}

template<bool IsConst>
std::uint64_t QueryBuilder<IsConst>::unsafeAsUint64() noexcept {
    assert(m_json != nullptr);

    return m_json->unsafeAsUint64();
}

template<bool IsConst>
typename QueryBuilder<IsConst>::ObjectRef QueryBuilder<IsConst>::unsafeAsObject() noexcept {
    assert(m_json != nullptr);

    return m_json->unsafeAsObject();
}

template<bool IsConst>
typename QueryBuilder<IsConst>::ArrayRef QueryBuilder<IsConst>::unsafeAsArray() noexcept {
    assert(m_json != nullptr);

    return m_json->unsafeAsArray();
}

template<bool IsConst>
std::nullptr_t QueryBuilder<IsConst>::unsafeAsNull() noexcept {
    assert(m_json != nullptr);

    return m_json->unsafeAsNull();
}

template<bool IsConst>
bool QueryBuilder<IsConst>::unsafeAsBool() noexcept {
    assert(m_json != nullptr);

    return m_json->unsafeAsBool();
}

}