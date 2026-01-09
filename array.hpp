#pragma once

#include <vector>
#include <cstdint>
#include <functional>

#include "allocator.hpp"

namespace CPPJSON {

class Parser;
class JSON;
class Object;
class String;

class Array {

template<typename T>
using GetCallbackRef = std::function<void(T&)>;

template<typename T>
using GetCallback = std::function<void(T)>;

typedef std::function<void()> FailureCallback;

public:
    typedef JSON                                  ValueType;
    typedef ValueType                             ContainerType;
    typedef ArenaAllocator<ContainerType>         Allocator;
    typedef std::vector<ContainerType, Allocator> Container;

    typedef ValueType*       iterator;
    typedef const ValueType* const_iterator;

    static const unsigned MINIMUM_CAPACITY;

    Array(const Allocator&);
    Array(Allocator&&)             noexcept;
    Array(const Array&)                     = default;
    Array(Array&&)                 noexcept = default;
    ~Array()                       noexcept;
    Array &operator=(const Array&);
    Array &operator=(Array&&)      noexcept;
    
    bool             reserve(unsigned capacity = 0U) noexcept;
    ValueType       *get    (unsigned)               noexcept;
    const ValueType *get    (unsigned)         const noexcept;

    String          *getString (unsigned index, bool &success)       noexcept;
    double           getFloat64(unsigned index, bool &success) const noexcept;
    std::int64_t     getInt64  (unsigned index, bool &success) const noexcept;
    std::uint64_t    getUint64 (unsigned index, bool &success) const noexcept;
    Object          *getObject (unsigned index, bool &success)       noexcept;
    Array           *getArray  (unsigned index, bool &success)       noexcept;
    std::nullptr_t   getNull   (unsigned index, bool &success) const noexcept;
    bool             getBool   (unsigned index, bool &success) const noexcept;

    String          *getString (unsigned index)       noexcept;
    double           getFloat64(unsigned index) const noexcept;
    std::int64_t     getInt64  (unsigned index) const noexcept;
    std::uint64_t    getUint64 (unsigned index) const noexcept;
    Object          *getObject (unsigned index)       noexcept;
    Array           *getArray  (unsigned index)       noexcept;
    std::nullptr_t   getNull   (unsigned index) const noexcept;
    bool             getBool   (unsigned index) const noexcept;

    bool getString (unsigned index, GetCallbackRef<String>)            noexcept;
    bool getFloat64(unsigned index, GetCallback<double>)         const noexcept;
    bool getInt64  (unsigned index, GetCallback<std::int64_t>)   const noexcept;
    bool getUint64 (unsigned index, GetCallback<std::uint64_t>)  const noexcept;
    bool getObject (unsigned index, GetCallbackRef<Object>)            noexcept;
    bool getArray  (unsigned index, GetCallbackRef<Array>)             noexcept;
    bool getNull   (unsigned index, GetCallback<std::nullptr_t>) const noexcept;
    bool getBool   (unsigned index, GetCallback<bool>)           const noexcept;

    bool getString (unsigned index, GetCallbackRef<String>,      FailureCallback)       noexcept;
    bool getFloat64(unsigned index, GetCallback<double>,         FailureCallback) const noexcept;
    bool getInt64  (unsigned index, GetCallback<std::int64_t>,   FailureCallback) const noexcept;
    bool getUint64 (unsigned index, GetCallback<std::uint64_t>,  FailureCallback) const noexcept;
    bool getObject (unsigned index, GetCallbackRef<Object>,      FailureCallback)       noexcept;
    bool getArray  (unsigned index, GetCallbackRef<Array>,       FailureCallback)       noexcept;
    bool getNull   (unsigned index, GetCallback<std::nullptr_t>, FailureCallback) const noexcept;
    bool getBool   (unsigned index, GetCallback<bool>,           FailureCallback) const noexcept;

    bool getString (unsigned index, GetCallbackRef<String>,      std::nullptr_t)       noexcept = delete;
    bool getFloat64(unsigned index, GetCallback<double>,         std::nullptr_t) const noexcept = delete;
    bool getInt64  (unsigned index, GetCallback<std::int64_t>,   std::nullptr_t) const noexcept = delete;
    bool getUint64 (unsigned index, GetCallback<std::uint64_t>,  std::nullptr_t) const noexcept = delete;
    bool getObject (unsigned index, GetCallbackRef<Object>,      std::nullptr_t)       noexcept = delete;
    bool getArray  (unsigned index, GetCallbackRef<Array>,       std::nullptr_t)       noexcept = delete;
    bool getNull   (unsigned index, GetCallback<std::nullptr_t>, std::nullptr_t) const noexcept = delete;
    bool getBool   (unsigned index, GetCallback<bool>,           std::nullptr_t) const noexcept = delete;

    bool getString (unsigned index, std::nullptr_t)       noexcept = delete;
    bool getFloat64(unsigned index, std::nullptr_t)       noexcept = delete;
    bool getInt64  (unsigned index, std::nullptr_t) const noexcept = delete;
    bool getUint64 (unsigned index, std::nullptr_t) const noexcept = delete;
    bool getObject (unsigned index, std::nullptr_t)       noexcept = delete;
    bool getArray  (unsigned index, std::nullptr_t)       noexcept = delete;
    bool getNull   (unsigned index, std::nullptr_t) const noexcept = delete;
    bool getBool   (unsigned index, std::nullptr_t) const noexcept = delete;

    bool set(unsigned index, String&&)                 noexcept; 
    bool set(unsigned index, double)                   noexcept; 
    bool set(unsigned index, std::int64_t)             noexcept; 
    bool set(unsigned index, std::uint64_t)            noexcept;
    bool set(unsigned index, int)                      noexcept;
    bool set(unsigned index, unsigned)                 noexcept;
    bool set(unsigned index, Object&&)                 noexcept;
    bool set(unsigned index, Array&&)                  noexcept;
    bool set(unsigned index, std::nullptr_t = nullptr) noexcept;
    bool set(unsigned index, bool)                     noexcept;

    bool push(String&&)                 noexcept;
    bool push(double)                   noexcept;
    bool push(std::int64_t)             noexcept;
    bool push(std::uint64_t)            noexcept;
    bool push(int)                      noexcept;
    bool push(unsigned)                 noexcept;
    bool push(Object&&)                 noexcept;
    bool push(Array&&)                  noexcept;
    bool push(std::nullptr_t = nullptr) noexcept;
    bool push(bool)                     noexcept;
    
    ValueType *back      ()       noexcept;
    ValueType &unsafeBack()       noexcept;
    unsigned   size      () const noexcept;

    ValueType       *operator[](unsigned)       noexcept;
    const ValueType *operator[](unsigned) const noexcept;

    Allocator getAllocator() const noexcept;

    iterator       begin()       noexcept;
    const_iterator begin() const noexcept;
    iterator       end()         noexcept;
    const_iterator end()   const noexcept;

private:
    Container m_data;

    JSON *getEntry(unsigned index) noexcept;

    template<typename T, T*(JSON::*method)(bool&)>
    T *get(const unsigned index, bool &success) noexcept {
        ValueType *const json = get(index);
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
    T get(const unsigned index, bool &success) const noexcept {
        const ValueType *const json = get(index);
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
    bool get(const unsigned index, GetCallbackRef<T> callback, FailureCallback failureCallback) noexcept {
        assert(callback != nullptr);
        assert(failureCallback != nullptr);

        bool success;
        T *const value = get<T, method>(index, success);

        if(success) {
            callback(*value);
        } else {
            failureCallback();
        }

        return success;
    }

    template<typename T, T*(JSON::*method)(bool&)>
    bool get(const unsigned index, GetCallbackRef<T> callback) noexcept {
        assert(callback != nullptr);

        bool success;
        T *const value = get<T, method>(index, success);

        if(success) {
            callback(*value);
        }

        return success;
    }

    template<typename T, T(JSON::*method)(bool&) const>
    bool get(const unsigned index, GetCallback<T> callback, FailureCallback failureCallback) const noexcept {
        assert(callback != nullptr);
        assert(failureCallback != nullptr);

        bool success;
        const T value = get<T, method>(index, success);

        if(success) {
            callback(value);
        } else {
            failureCallback();
        }

        return success;
    }

    template<typename T, T(JSON::*method)(bool&) const>
    bool get(const unsigned index, GetCallback<T> callback) const noexcept {
        assert(callback != nullptr);

        bool success;
        const T value = get<T, method>(index, success);

        if(success) {
            callback(value);
        }

        return success;
    }
};

}