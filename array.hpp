#pragma once

#include <vector>
#include <cstdint>

#include "allocator.hpp"
#include "result.hpp"

namespace CPPJSON {

class Parser;
class JSON;
class Object;
class String;

class Array {

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
    
    bool reserve(unsigned capacity = 0U) noexcept;

    Result<JSON&>          get       (unsigned index)       noexcept;
    Result<double>         getFloat64(unsigned index) const noexcept;
    Result<String&>        getString (unsigned index)       noexcept;
    Result<std::int64_t>   getInt64  (unsigned index) const noexcept;
    Result<std::uint64_t>  getUint64 (unsigned index) const noexcept;
    Result<Object&>        getObject (unsigned index)       noexcept;
    Result<Array&>         getArray  (unsigned index)       noexcept;
    Result<std::nullptr_t> getNull   (unsigned index) const noexcept;
    Result<bool>           getBool   (unsigned index) const noexcept;

    Result<const JSON&>   get       (unsigned index) const noexcept;
    Result<const String&> getString (unsigned index) const noexcept;
    Result<const Object&> getObject (unsigned index) const noexcept;
    Result<const Array&>  getArray  (unsigned index) const noexcept;

    JSON          &unsafeGet       (unsigned index)       noexcept;
    String        &unsafeGetString (unsigned index)       noexcept;
    double         unsafeGetFloat64(unsigned index) const noexcept;
    std::int64_t   unsafeGetInt64  (unsigned index) const noexcept;
    std::uint64_t  unsafeGetUint64 (unsigned index) const noexcept;
    Object        &unsafeGetObject (unsigned index)       noexcept;
    Array         &unsafeGetArray  (unsigned index)       noexcept;
    std::nullptr_t unsafeGetNull   (unsigned index) const noexcept;
    bool           unsafeGetBool   (unsigned index) const noexcept;

    const JSON   &unsafeGet       (unsigned index) const noexcept;
    const String &unsafeGetString (unsigned index) const noexcept;
    const Object &unsafeGetObject (unsigned index) const noexcept;
    const Array  &unsafeGetArray  (unsigned index) const noexcept;

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
    
    Result<JSON&>       back      ()       noexcept;
    JSON               &unsafeBack()       noexcept;
    Result<const JSON&> back      () const noexcept;
    const JSON         &unsafeBack() const noexcept;
    unsigned            size      () const noexcept;

    JSON&       operator[](unsigned);
    const JSON& operator[](unsigned) const noexcept;

    Allocator getAllocator() const noexcept;

    iterator       begin()       noexcept;
    const_iterator begin() const noexcept;
    iterator       end()         noexcept;
    const_iterator end()   const noexcept;

    void destructor() noexcept;

private:
    Container m_data;

    JSON       *getEntry(unsigned index);
    const JSON *getEntry(unsigned index) const noexcept;

    template<typename T, Result<T&>(JSON::*method)()>
    Result<T&> getRef(const unsigned index) noexcept {
        const Result<JSON&> json = get(index);
        if(!json.isSuccess()) {
            return Result<T&>::fromError(true);
        }

        return (json.getRef().*method)();
    }

    template<typename T, Result<T&>(JSON::*method)()>
    Result<T> get(const unsigned index) noexcept {
        const Result<JSON&> json = get(index);
        if(!json.isSuccess()) {
            return Result<T&>::fromError(true);
        }

        return (json.getRef().*method)();
    }

    template<typename T, Result<const T&>(JSON::*method)() const>
    Result<const T&> getConstRef(const unsigned index) const noexcept {
        const Result<const JSON&> json = get(index);
        if(!json.isSuccess()) {
            return Result<const T&>::fromError(true);
        }

        return (json.getRef().*method)();
    }

    template<typename T, Result<T>(JSON::*method)() const>
    Result<T> getConst(const unsigned index) const noexcept {
        const Result<const JSON&> json = get(index);
        if(!json.isSuccess()) {
            return Result<T>::fromError(true);
        }

        return (json.getRef().*method)();
    }

    template<typename T>
    bool setValue(const unsigned index, T value) noexcept;

    template<typename T>
    bool setRef(const unsigned index, T &&value, typename std::enable_if<!std::is_lvalue_reference<T>::value>::type* = nullptr) noexcept;

    template<typename T>
    bool pushValue(T value) noexcept;

    template<typename T>
    bool pushRef(T &&value, typename std::enable_if<!std::is_lvalue_reference<T>::value>::type* = nullptr) noexcept;
};

}