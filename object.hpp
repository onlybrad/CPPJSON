#pragma once

#include <cassert>
#include <unordered_map>
#include <cstdint>

#include "allocator.hpp"
#include "string.hpp"
#include "result.hpp"

template<>
struct std::hash<CPPJSON::String> {
    std::size_t operator()(const CPPJSON::String &string) const noexcept {
        assert(string.size() <= static_cast<std::size_t>(std::numeric_limits<unsigned>().max()));

        unsigned h = 0U;

        for(const CPPJSON::String::ContainerType c : string) {
            h += static_cast<unsigned>(c);
            h += h << 10;
            h ^= h >> 6;
        }

        h += h << 3;
        h ^= h >> 11;
        h += h << 15;

        return h;
    }
};

template<>
struct std::equal_to<CPPJSON::String> {
    bool operator()(const CPPJSON::String& lhs, const CPPJSON::String& rhs) const {
        return lhs == rhs;
    }
};

namespace CPPJSON {

class Parser;
class JSON;
class Array;

class Object {

public:
    typedef String                              KeyType;
    typedef JSON                                ValueType;
    typedef std::pair<const KeyType, ValueType> KeyValueType;
    typedef KeyValueType                        ContainerType;
    typedef ArenaAllocator<ContainerType>       Allocator;
    typedef ArenaAllocator<KeyType>             KeyAllocator;
    typedef std::unordered_map<
        KeyType,
        ValueType,
        std::hash<KeyType>,
        std::equal_to<KeyType>,
        Allocator
    > Container;

    typedef Container::iterator       iterator;
    typedef Container::const_iterator const_iterator;

    static const unsigned MINIMUM_CAPACITY; 
    
    Object(const Allocator&);
    Object(Allocator&&)              noexcept;
    Object(const Object&)                     = default;
    Object(Object&&)                 noexcept = default;
    ~Object()                        noexcept;
    Object &operator=(const Object&);
    Object &operator=(Object&&)      noexcept;
    
    bool reserve(unsigned capacity = 0U) noexcept;
    void remove (const std::string&)     noexcept;
    void remove (const String&)          noexcept;
    void remove (const char*)            noexcept;
    
    Result<JSON&>   get      (const std::string&) noexcept;
    Result<String&> getString(const std::string&) noexcept;
    Result<Object&> getObject(const std::string&) noexcept;
    Result<Array&>  getArray (const std::string&) noexcept;
    Result<JSON&>   get      (const String&)      noexcept;
    Result<String&> getString(const String&)      noexcept;
    Result<Object&> getObject(const String&)      noexcept;
    Result<Array&>  getArray (const String&)      noexcept;
    Result<JSON&>   get      (const char*)        noexcept;
    Result<String&> getString(const char*)        noexcept;
    Result<Object&> getObject(const char*)        noexcept;
    Result<Array&>  getArray (const char*)        noexcept;
         
    Result<const JSON&>    get       (const std::string&) const noexcept;
    Result<const String&>  getString (const std::string&) const noexcept;
    Result<double>         getFloat64(const std::string&) const noexcept;
    Result<std::int64_t>   getInt64  (const std::string&) const noexcept;
    Result<std::uint64_t>  getUint64 (const std::string&) const noexcept;
    Result<const Object&>  getObject (const std::string&) const noexcept;
    Result<const Array&>   getArray  (const std::string&) const noexcept;
    Result<std::nullptr_t> getNull   (const std::string&) const noexcept;
    Result<bool>           getBool   (const std::string&) const noexcept;
    Result<const JSON&>    get       (const String&)      const noexcept;
    Result<const String&>  getString (const String&)      const noexcept;
    Result<double>         getFloat64(const String&)      const noexcept;
    Result<std::int64_t>   getInt64  (const String&)      const noexcept;
    Result<std::uint64_t>  getUint64 (const String&)      const noexcept;
    Result<const Object&>  getObject (const String&)      const noexcept;
    Result<const Array&>   getArray  (const String&)      const noexcept;
    Result<std::nullptr_t> getNull   (const String&)      const noexcept;
    Result<bool>           getBool   (const String&)      const noexcept;
    Result<const JSON&>    get       (const char*)        const noexcept;
    Result<const String&>  getString (const char*)        const noexcept;
    Result<double>         getFloat64(const char*)        const noexcept;
    Result<std::int64_t>   getInt64  (const char*)        const noexcept;
    Result<std::uint64_t>  getUint64 (const char*)        const noexcept;
    Result<const Object&>  getObject (const char*)        const noexcept;
    Result<const Array&>   getArray  (const char*)        const noexcept;
    Result<std::nullptr_t> getNull   (const char*)        const noexcept;
    Result<bool>           getBool   (const char*)        const noexcept;
        
    JSON   &unsafeGet      (const std::string&);
    String &unsafeGetString(const std::string&);
    Object &unsafeGetObject(const std::string&);
    Array  &unsafeGetArray (const std::string&);
    JSON   &unsafeGet      (const String&);
    String &unsafeGetString(const String&);
    Object &unsafeGetObject(const String&);
    Array  &unsafeGetArray (const String&);
    JSON   &unsafeGet      (const char*);
    String &unsafeGetString(const char*);
    Object &unsafeGetObject(const char*);
    Array  &unsafeGetArray (const char*);
       
    const JSON    &unsafeGet       (const std::string&) const noexcept;
    const String  &unsafeGetString (const std::string&) const noexcept;
    double         unsafeGetFloat64(const std::string&) const noexcept;
    std::int64_t   unsafeGetInt64  (const std::string&) const noexcept;
    std::uint64_t  unsafeGetUint64 (const std::string&) const noexcept;
    const Object  &unsafeGetObject (const std::string&) const noexcept;
    const Array   &unsafeGetArray  (const std::string&) const noexcept;
    std::nullptr_t unsafeGetNull   (const std::string&) const noexcept;
    bool           unsafeGetBool   (const std::string&) const noexcept;
    const JSON    &unsafeGet       (const String&)      const noexcept;
    const String  &unsafeGetString (const String&)      const noexcept;
    double         unsafeGetFloat64(const String&)      const noexcept;
    std::int64_t   unsafeGetInt64  (const String&)      const noexcept;
    std::uint64_t  unsafeGetUint64 (const String&)      const noexcept;
    const Object  &unsafeGetObject (const String&)      const noexcept;
    const Array   &unsafeGetArray  (const String&)      const noexcept;
    std::nullptr_t unsafeGetNull   (const String&)      const noexcept;
    bool           unsafeGetBool   (const String&)      const noexcept;
    const JSON    &unsafeGet       (const char*)        const noexcept;
    const String  &unsafeGetString (const char*)        const noexcept;
    double         unsafeGetFloat64(const char*)        const noexcept;
    std::int64_t   unsafeGetInt64  (const char*)        const noexcept;
    std::uint64_t  unsafeGetUint64 (const char*)        const noexcept;
    const Object  &unsafeGetObject (const char*)        const noexcept;
    const Array   &unsafeGetArray  (const char*)        const noexcept;
    std::nullptr_t unsafeGetNull   (const char*)        const noexcept;
    bool           unsafeGetBool   (const char*)        const noexcept;
        
    bool set(const String&, JSON&&)                   noexcept;
    bool set(const String&, String&&)                 noexcept;
    bool set(const String&, double)                   noexcept;
    bool set(const String&, std::int64_t)             noexcept;
    bool set(const String&, std::uint64_t)            noexcept;
    bool set(const String&, int)                      noexcept;
    bool set(const String&, unsigned)                 noexcept;
    bool set(const String&, Object&&)                 noexcept;
    bool set(const String&, Array&&)                  noexcept;
    bool set(const String&, std::nullptr_t = nullptr) noexcept;
    bool set(const String&, bool)                     noexcept;
    bool set(String&&, JSON&&)                        noexcept;
    bool set(String&&, String&&)                      noexcept;
    bool set(String&&, double)                        noexcept;
    bool set(String&&, std::int64_t)                  noexcept;
    bool set(String&&, std::uint64_t)                 noexcept;
    bool set(String&&, int)                           noexcept;
    bool set(String&&, unsigned)                      noexcept;
    bool set(String&&, Object&&)                      noexcept;
    bool set(String&&, Array&&)                       noexcept;
    bool set(String&&, std::nullptr_t = nullptr)      noexcept;
    bool set(String&&, bool)                          noexcept;

    bool     has (const std::string&) const noexcept;
    bool     has (const char*)        const noexcept;
    bool     has (const String&)      const noexcept;
    unsigned size() const noexcept;

    const JSON& operator[](const std::string&) const noexcept;
    const JSON& operator[](const char*)        const noexcept;
    JSON&       operator[](const String&);
    JSON&       operator[](String&&)                 noexcept;
    const JSON& operator[](const String&)      const noexcept;

    Allocator getAllocator() const noexcept;

    iterator       begin()       noexcept;
    const_iterator begin() const noexcept;
    iterator       end()         noexcept;
    const_iterator end()   const noexcept;

    void destructor() noexcept;

    static KeyAllocator &getKeyAllocator();
    
private:
    Container m_data;
    static Arena        s_keyArena;
    static KeyAllocator s_keyAllocator;

    template<typename T, Result<T&>(JSON::*method)()>
    Result<T&> getRef(const char *const key) noexcept {
        const Result<JSON&> jsonResult = get(key);
        if(!jsonResult.isSuccess()) {
            return Result<T&>::fromError(true);
        }

        return (jsonResult.getRef().*method)();
    }

    template<typename T, Result<const T&>(JSON::*method)() const>
    Result<const T&> getConstRef(const char *const key) const noexcept {
        const Result<const JSON&> jsonResult = get(key);
        if(!jsonResult.isSuccess()) {
            return Result<const T&>::fromError(true);
        }

        return (jsonResult.getRef().*method)();
    }

    template<typename T, Result<T>(JSON::*method)() const>
    Result<T> getValue(const char *const key) const noexcept {
        const Result<const JSON&> jsonResult = get(key);
        if(!jsonResult.isSuccess()) {
            return Result<T>::fromError(true);
        }

        return (jsonResult.getRef().*method)();
    }

    template<typename T, Result<T&>(JSON::*method)()>
    Result<T&> getRef(const String &key) noexcept {
        const Result<JSON&> jsonResult = get(key);
        if(!jsonResult.isSuccess()) {
            return Result<T&>::fromError(true);
        }

        return (jsonResult.getRef().*method)();
    }

    template<typename T, Result<const T&>(JSON::*method)() const>
    Result<const T&> getConstRef(const String &key) const noexcept {
        const Result<const JSON&> jsonResult = get(key);
        if(!jsonResult.isSuccess()) {
            return Result<const T&>::fromError(true);
        }

        return (jsonResult.getRef().*method)();
    }

    template<typename T, Result<T>(JSON::*method)() const>
    Result<T> getValue(const String &key) const noexcept {
        const Result<const JSON&> jsonResult = get(key);
        if(!jsonResult.isSuccess()) {
            return Result<T>::fromError(true);
        }

        return (jsonResult.getRef().*method)();
    }

    template<typename T>
    bool setValue(const String &key, const T value) noexcept {
        try {
            (*this)[key] = value;
            return true;
        } catch(...) {
            return false;
        }
    }

    template<typename T>
    bool setValue(String &&key, const T value) noexcept {
        try {
            (*this)[std::move(key)] = value;
            return true;
        } catch(...) {
            return false;
        }
    }

    template <typename T>
    bool setRef(const String &key, T &&value,
    typename std::enable_if<!std::is_lvalue_reference<T>::value>::type* = nullptr) noexcept {
        try {
            (*this)[key] = std::move(value);
            return true;
        } catch (...) {
            return false;
        }
    }

    template <typename T>
    bool setRef(String &&key, T &&value,
    typename std::enable_if<!std::is_lvalue_reference<T>::value>::type* = nullptr) noexcept {
        try {
            (*this)[std::move(key)] = std::move(value);
            return true;
        } catch (...) {
            return false;
        }
    }
};

}