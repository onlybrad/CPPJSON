#pragma once

#include <cassert>
#include <unordered_map>
#include <cstdint>
#include <functional>

#include "allocator.hpp"
#include "string.hpp"

namespace CPPJSON {

class Parser;
class JSON;
class Array;

class Object {

template<typename T>
using GetCallbackRef = std::function<void(T&)>;

template<typename T>
using GetCallback = std::function<void(T)>;

typedef std::function<void()> FailureCallback;

public:
    typedef String::Container                   KeyType;
    typedef JSON                                ValueType;
    typedef std::pair<const KeyType, ValueType> KeyValueType;
    typedef KeyValueType                        ContainerType;
    typedef ArenaAllocator<ContainerType>       Allocator;
    typedef std::unordered_map<
        KeyType,
        ValueType,
        std::hash<KeyType>,
        std::equal_to<KeyType>,
        Allocator
    > Container;
   
    static const unsigned MINIMUM_CAPACITY; 
    
    Object(const Allocator&);
    Object(Allocator&&)              noexcept;
    Object(const Object&)                     = default;
    Object(Object&&)                 noexcept = default;
    ~Object()                        noexcept;
    Object &operator=(const Object&);
    Object &operator=(Object&&)      noexcept;

    bool           reserve(unsigned capacity = 0U) noexcept;

    ValueType       *get(const std::string&)      noexcept;
    ValueType       *get(const String&)           noexcept;
    ValueType       *get(const char *const)       noexcept;
    const ValueType *get(const char *const) const noexcept;

    void           remove(const std::string&) noexcept;
    void           remove(const String&)      noexcept;
    void           remove(const char *const)  noexcept;

    String        *getString (const std::string&, bool &success)       noexcept;
    double         getFloat64(const std::string&, bool &success) const noexcept;
    std::int64_t   getInt64  (const std::string&, bool &success) const noexcept;
    std::uint64_t  getUint64 (const std::string&, bool &success) const noexcept;
    Object        *getObject (const std::string&, bool &success)       noexcept;
    Array         *getArray  (const std::string&, bool &success)       noexcept;
    std::nullptr_t getNull   (const std::string&, bool &success) const noexcept;
    bool           getBool   (const std::string&, bool &success) const noexcept;

    String        *getString (const String&, bool &success)       noexcept;
    double         getFloat64(const String&, bool &success) const noexcept;
    std::int64_t   getInt64  (const String&, bool &success) const noexcept;
    std::uint64_t  getUint64 (const String&, bool &success) const noexcept;
    Object        *getObject (const String&, bool &success)       noexcept;
    Array         *getArray  (const String&, bool &success)       noexcept;
    std::nullptr_t getNull   (const String&, bool &success) const noexcept;
    bool           getBool   (const String&, bool &success) const noexcept;
    
    String        *getString (const char*, bool &success)       noexcept;
    double         getFloat64(const char*, bool &success) const noexcept;
    std::int64_t   getInt64  (const char*, bool &success) const noexcept;
    std::uint64_t  getUint64 (const char*, bool &success) const noexcept;
    Object        *getObject (const char*, bool &success)       noexcept;
    Array         *getArray  (const char*, bool &success)       noexcept;
    std::nullptr_t getNull   (const char*, bool &success) const noexcept;
    bool           getBool   (const char*, bool &success) const noexcept;
    
    String        *getString (const std::string&)       noexcept;
    double         getFloat64(const std::string&) const noexcept;
    std::int64_t   getInt64  (const std::string&) const noexcept;
    std::uint64_t  getUint64 (const std::string&) const noexcept;
    Object        *getObject (const std::string&)       noexcept;
    Array         *getArray  (const std::string&)       noexcept;
    std::nullptr_t getNull   (const std::string&) const noexcept;
    bool           getBool   (const std::string&) const noexcept;
    
    String        *getString (const String&)       noexcept;
    double         getFloat64(const String&) const noexcept;
    std::int64_t   getInt64  (const String&) const noexcept;
    std::uint64_t  getUint64 (const String&) const noexcept;
    Object        *getObject (const String&)       noexcept;
    Array         *getArray  (const String&)       noexcept;
    std::nullptr_t getNull   (const String&) const noexcept;
    bool           getBool   (const String&) const noexcept;
    
    String        *getString (const char*)       noexcept;
    double         getFloat64(const char*) const noexcept;
    std::int64_t   getInt64  (const char*) const noexcept;
    std::uint64_t  getUint64 (const char*) const noexcept;
    Object        *getObject (const char*)       noexcept;
    Array         *getArray  (const char*)       noexcept;
    std::nullptr_t getNull   (const char*) const noexcept;
    bool           getBool   (const char*) const noexcept;

    bool  getString (const std::string&, GetCallbackRef<String>)            noexcept;
    bool  getFloat64(const std::string&, GetCallback<double>)         const noexcept;
    bool  getInt64  (const std::string&, GetCallback<std::int64_t>)   const noexcept;
    bool  getUint64 (const std::string&, GetCallback<std::uint64_t>)  const noexcept;
    bool  getObject (const std::string&, GetCallbackRef<Object>)            noexcept;
    bool  getArray  (const std::string&, GetCallbackRef<Array>)             noexcept;
    bool  getNull   (const std::string&, GetCallback<std::nullptr_t>) const noexcept;
    bool  getBool   (const std::string&, GetCallback<bool>)           const noexcept;

    bool  getString (const std::string&, GetCallbackRef<String>,      FailureCallback)       noexcept;
    bool  getFloat64(const std::string&, GetCallback<double>,         FailureCallback) const noexcept;
    bool  getInt64  (const std::string&, GetCallback<std::int64_t>,   FailureCallback) const noexcept;
    bool  getUint64 (const std::string&, GetCallback<std::uint64_t>,  FailureCallback) const noexcept;
    bool  getObject (const std::string&, GetCallbackRef<Object>,      FailureCallback)       noexcept;
    bool  getArray  (const std::string&, GetCallbackRef<Array>,       FailureCallback)       noexcept;
    bool  getNull   (const std::string&, GetCallback<std::nullptr_t>, FailureCallback) const noexcept;
    bool  getBool   (const std::string&, GetCallback<bool>,           FailureCallback) const noexcept;

    bool  getString (const std::string&, GetCallbackRef<String>,      std::nullptr_t)       noexcept = delete;
    bool  getFloat64(const std::string&, GetCallback<double>,         std::nullptr_t) const noexcept = delete;
    bool  getInt64  (const std::string&, GetCallback<std::int64_t>,   std::nullptr_t) const noexcept = delete;
    bool  getUint64 (const std::string&, GetCallback<std::uint64_t>,  std::nullptr_t) const noexcept = delete;
    bool  getObject (const std::string&, GetCallbackRef<Object>,      std::nullptr_t)       noexcept = delete;
    bool  getArray  (const std::string&, GetCallbackRef<Array>,       std::nullptr_t)       noexcept = delete;
    bool  getNull   (const std::string&, GetCallback<std::nullptr_t>, std::nullptr_t) const noexcept = delete;
    bool  getBool   (const std::string&, GetCallback<bool>,           std::nullptr_t) const noexcept = delete;

    bool  getString (const std::string&, std::nullptr_t)       noexcept = delete;
    bool  getFloat64(const std::string&, std::nullptr_t) const noexcept = delete;
    bool  getInt64  (const std::string&, std::nullptr_t) const noexcept = delete;
    bool  getUint64 (const std::string&, std::nullptr_t) const noexcept = delete;
    bool  getObject (const std::string&, std::nullptr_t)       noexcept = delete;
    bool  getArray  (const std::string&, std::nullptr_t)       noexcept = delete;
    bool  getNull   (const std::string&, std::nullptr_t) const noexcept = delete;
    bool  getBool   (const std::string&, std::nullptr_t) const noexcept = delete;

    bool  getString (const String&, GetCallbackRef<String>)            noexcept;
    bool  getFloat64(const String&, GetCallback<double>)         const noexcept;
    bool  getInt64  (const String&, GetCallback<std::int64_t>)   const noexcept;
    bool  getUint64 (const String&, GetCallback<std::uint64_t>)  const noexcept;
    bool  getObject (const String&, GetCallbackRef<Object>)            noexcept;
    bool  getArray  (const String&, GetCallbackRef<Array>)             noexcept;
    bool  getNull   (const String&, GetCallback<std::nullptr_t>) const noexcept;
    bool  getBool   (const String&, GetCallback<bool>)           const noexcept;

    bool  getString (const String&, GetCallbackRef<String>,      FailureCallback)       noexcept;
    bool  getFloat64(const String&, GetCallback<double>,         FailureCallback) const noexcept;
    bool  getInt64  (const String&, GetCallback<std::int64_t>,   FailureCallback) const noexcept;
    bool  getUint64 (const String&, GetCallback<std::uint64_t>,  FailureCallback) const noexcept;
    bool  getObject (const String&, GetCallbackRef<Object>,      FailureCallback)       noexcept;
    bool  getArray  (const String&, GetCallbackRef<Array>,       FailureCallback)       noexcept;
    bool  getNull   (const String&, GetCallback<std::nullptr_t>, FailureCallback) const noexcept;
    bool  getBool   (const String&, GetCallback<bool>,           FailureCallback) const noexcept;

    bool  getString (const String&, GetCallbackRef<String>,      std::nullptr_t)       noexcept = delete;
    bool  getFloat64(const String&, GetCallback<double>,         std::nullptr_t) const noexcept = delete;
    bool  getInt64  (const String&, GetCallback<std::int64_t>,   std::nullptr_t) const noexcept = delete;
    bool  getUint64 (const String&, GetCallback<std::uint64_t>,  std::nullptr_t) const noexcept = delete;
    bool  getObject (const String&, GetCallbackRef<Object>,      std::nullptr_t)       noexcept = delete;
    bool  getArray  (const String&, GetCallbackRef<Array>,       std::nullptr_t)       noexcept = delete;
    bool  getNull   (const String&, GetCallback<std::nullptr_t>, std::nullptr_t) const noexcept = delete;
    bool  getBool   (const String&, GetCallback<bool>,           std::nullptr_t) const noexcept = delete;

    bool  getString (const String&, std::nullptr_t)       noexcept = delete;
    bool  getFloat64(const String&, std::nullptr_t) const noexcept = delete;
    bool  getInt64  (const String&, std::nullptr_t) const noexcept = delete;
    bool  getUint64 (const String&, std::nullptr_t) const noexcept = delete;
    bool  getObject (const String&, std::nullptr_t)       noexcept = delete;
    bool  getArray  (const String&, std::nullptr_t)       noexcept = delete;
    bool  getNull   (const String&, std::nullptr_t) const noexcept = delete;
    bool  getBool   (const String&, std::nullptr_t) const noexcept = delete;

    bool  getString (const char*, GetCallbackRef<String>)            noexcept;
    bool  getFloat64(const char*, GetCallback<double>)         const noexcept;
    bool  getInt64  (const char*, GetCallback<std::int64_t>)   const noexcept;
    bool  getUint64 (const char*, GetCallback<std::uint64_t>)  const noexcept;
    bool  getObject (const char*, GetCallbackRef<Object>)            noexcept;
    bool  getArray  (const char*, GetCallbackRef<Array>)             noexcept;
    bool  getNull   (const char*, GetCallback<std::nullptr_t>) const noexcept;
    bool  getBool   (const char*, GetCallback<bool>)           const noexcept;

    bool  getString (const char*, GetCallbackRef<String>,      FailureCallback)       noexcept;
    bool  getFloat64(const char*, GetCallback<double>,         FailureCallback) const noexcept;
    bool  getInt64  (const char*, GetCallback<std::int64_t>,   FailureCallback) const noexcept;
    bool  getUint64 (const char*, GetCallback<std::uint64_t>,  FailureCallback) const noexcept;
    bool  getObject (const char*, GetCallbackRef<Object>,      FailureCallback)       noexcept;
    bool  getArray  (const char*, GetCallbackRef<Array>,       FailureCallback)       noexcept;
    bool  getNull   (const char*, GetCallback<std::nullptr_t>, FailureCallback) const noexcept;
    bool  getBool   (const char*, GetCallback<bool>,           FailureCallback) const noexcept;

    bool  getString (const char*, GetCallbackRef<String>,      std::nullptr_t)       noexcept = delete;
    bool  getFloat64(const char*, GetCallback<double>,         std::nullptr_t) const noexcept = delete;
    bool  getInt64  (const char*, GetCallback<std::int64_t>,   std::nullptr_t) const noexcept = delete;
    bool  getUint64 (const char*, GetCallback<std::uint64_t>,  std::nullptr_t) const noexcept = delete;
    bool  getObject (const char*, GetCallbackRef<Object>,      std::nullptr_t)       noexcept = delete;
    bool  getArray  (const char*, GetCallbackRef<Array>,       std::nullptr_t)       noexcept = delete;
    bool  getNull   (const char*, GetCallback<std::nullptr_t>, std::nullptr_t) const noexcept = delete;
    bool  getBool   (const char*, GetCallback<bool>,           std::nullptr_t) const noexcept = delete;

    bool  getString (const char*, std::nullptr_t)       noexcept = delete;
    bool  getFloat64(const char*, std::nullptr_t) const noexcept = delete;
    bool  getInt64  (const char*, std::nullptr_t) const noexcept = delete;
    bool  getUint64 (const char*, std::nullptr_t) const noexcept = delete;
    bool  getObject (const char*, std::nullptr_t)       noexcept = delete;
    bool  getArray  (const char*, std::nullptr_t)       noexcept = delete;
    bool  getNull   (const char*, std::nullptr_t) const noexcept = delete;
    bool  getBool   (const char*, std::nullptr_t) const noexcept = delete;
    
    bool set(const std::string&, JSON&&)                   noexcept;
    bool set(const std::string&, String&&)                 noexcept;
    bool set(const std::string&, double)                   noexcept;
    bool set(const std::string&, int64_t)                  noexcept;
    bool set(const std::string&, uint64_t)                 noexcept;
    bool set(const std::string&, int)                      noexcept;
    bool set(const std::string&, unsigned)                 noexcept;
    bool set(const std::string&, Object&&)                 noexcept;
    bool set(const std::string&, Array&&)                  noexcept;
    bool set(const std::string&, std::nullptr_t = nullptr) noexcept;
    bool set(const std::string&, bool)                     noexcept;
    
    bool set(const String&, JSON&&)                   noexcept;
    bool set(const String&, String&&)                 noexcept;
    bool set(const String&, double)                   noexcept;
    bool set(const String&, int64_t)                  noexcept;
    bool set(const String&, uint64_t)                 noexcept;
    bool set(const String&, int)                      noexcept;
    bool set(const String&, unsigned)                 noexcept;
    bool set(const String&, Object&&)                 noexcept;
    bool set(const String&, Array&&)                  noexcept;
    bool set(const String&, std::nullptr_t = nullptr) noexcept;
    bool set(const String&, bool)                     noexcept;
    
    bool set(const char*, JSON&&)                   noexcept;
    bool set(const char*, String&&)                 noexcept;
    bool set(const char*, double)                   noexcept;
    bool set(const char*, int64_t)                  noexcept;
    bool set(const char*, uint64_t)                 noexcept;
    bool set(const char*, int)                      noexcept;
    bool set(const char*, unsigned)                 noexcept;
    bool set(const char*, Object&&)                 noexcept;
    bool set(const char*, Array&&)                  noexcept;
    bool set(const char*, std::nullptr_t = nullptr) noexcept;
    bool set(const char*, bool)                     noexcept;

    ValueType *operator[](const std::string&)  noexcept;
    ValueType *operator[](const String&)       noexcept;
    ValueType *operator[](String&&)            noexcept;
    ValueType *operator[](String::Container&&) noexcept;
    ValueType *operator[](const char*)         noexcept;

    Allocator getAllocator() const noexcept;
    
private:
    Container m_data;

    template<typename T, T*(JSON::*method)(bool&)>
    T *get(const char *const key, bool &success) noexcept {
        assert(key != nullptr);

        ValueType *const json = get(key);
        if(json == nullptr) {
            success = false;
            return nullptr;
        }

        T *const ret = (json->*method)(success);
        if(!success) {
            success = false;
            return nullptr;
        }

        success = true;
        return ret;
    }

    template<typename T, T(JSON::*method)(bool&) const>
    T get(const char *const key, bool &success) const noexcept {
        assert(key != nullptr);

        const ValueType *const json = get(key);
        if(json == nullptr) {
            success = false;
            return 0;
        }

        const T ret = (json->*method)(success);
        if(!success) {
            success = false;
            return 0;
        }

        success = true;
        return ret;
    }

    template<typename T, T*(JSON::*method)(bool&)>
    bool get(const char *const key, GetCallbackRef<T> callback, FailureCallback failureCallback) noexcept {
        assert(key != nullptr);
        assert(callback != nullptr);
        assert(failureCallback != nullptr);

        bool success;
        T *const value = get<T, method>(key, success);

        if(success) {
            callback(*value);
        } else {
            failureCallback();
        }

        return success;
    }

    template<typename T, T*(JSON::*method)(bool&)>
    bool get(const char *const key, GetCallbackRef<T> callback) noexcept {
        assert(key != nullptr);
        assert(callback != nullptr);

        bool success;
        T *const value = get<T, method>(key, success);

        if(success) {
            callback(*value);
        }

        return success;
    }

    template<typename T, T(JSON::*method)(bool&) const>
    bool get(const char *const key, GetCallback<T> callback, FailureCallback failureCallback) const noexcept {
        assert(key != nullptr);
        assert(callback != nullptr);
        assert(failureCallback != nullptr);

        bool success;
        const T value = get<T, method>(key, success);

        if(success) {
            callback(value);
        } else {
            failureCallback();
        }

        return success;
    }

    template<typename T, T(JSON::*method)(bool&) const>
    bool get(const char *const key, GetCallback<T> callback) const noexcept {
        assert(key != nullptr);
        assert(callback != nullptr);

        bool success;
        const T value = get<T, method>(key, success);

        if(success) {
            callback(value);
        }

        return success;
    }

    template<typename T, T&(JSON::*method)(bool&)>
    bool get(const std::string &key, GetCallbackRef<T> callback, FailureCallback failureCallback) noexcept {
        assert(callback != nullptr);
        assert(failureCallback != nullptr);

        return get<T,method>(key.c_str(), callback, failureCallback);
    }

    template<typename T, T&(JSON::*method)(bool&)>
    bool get(const String &key, GetCallbackRef<T> callback, FailureCallback failureCallback) noexcept {
        assert(callback != nullptr);
        assert(failureCallback != nullptr);

        return get<T,method>(key.getCStr(), callback, failureCallback);
    }

    template<typename T, T(JSON::*method)(bool&) const>
    bool get(const std::string &key, GetCallback<T> callback, FailureCallback failureCallback) const noexcept {
        assert(callback != nullptr);
        assert(failureCallback != nullptr);

        return get<T,method>(key.c_str(), callback, failureCallback);
    }

    template<typename T, T(JSON::*method)(bool&) const>
    bool get(const String &key, GetCallback<T> callback, FailureCallback failureCallback) const noexcept {
        assert(callback != nullptr);
        assert(failureCallback != nullptr);

        return get<T,method>(key.getCStr(), callback, failureCallback);
    }    

    template<typename T, T&(JSON::*method)(bool&)>
    bool get(const std::string &key, GetCallbackRef<T> callback) noexcept {
        assert(callback != nullptr);

        return get<T,method>(key.c_str(), callback);
    }

    template<typename T, T&(JSON::*method)(bool&)>
    bool get(const String &key, GetCallbackRef<T> callback) noexcept {
        assert(callback != nullptr);

        return get<T,method>(key.getCStr(), callback);
    }

    template<typename T, T(JSON::*method)(bool&) const>
    bool get(const std::string &key, GetCallback<T> callback) const noexcept {
        assert(callback != nullptr);

        return get<T,method>(key.c_str(), callback);
    }

    template<typename T, T(JSON::*method)(bool&) const>
    bool get(const String &key, GetCallback<T> callback) const noexcept {
        assert(callback != nullptr);

        return get<T,method>(key.getCStr(), callback);
    }
};

}