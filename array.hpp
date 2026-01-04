#pragma once

#include <vector>
#include <cstdint>

#include "allocator.hpp"

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
    
    bool             reserve   (unsigned capacity = 0U)                noexcept;
    ValueType       *get       (unsigned)                              noexcept;
    const ValueType *get       (unsigned)                        const noexcept;
    String          *getString (unsigned index, bool &success)               noexcept;
    double           getFloat64(unsigned index, bool &success)               noexcept;
    std::int64_t     getInt64  (unsigned index, bool &success)               noexcept;
    std::uint64_t    getUint64 (unsigned index, bool &success)               noexcept;
    Object          *getObject (unsigned index, bool &success)               noexcept;
    Array           *getArray  (unsigned index, bool &success)               noexcept;
    std::nullptr_t   getNull   (unsigned index, bool &success)               noexcept;
    bool             getBool   (unsigned index, bool &success)               noexcept;
    bool             set       (unsigned index, String&&)                    noexcept; 
    bool             set       (unsigned index, double)                      noexcept; 
    bool             set       (unsigned index, int64_t)                     noexcept; 
    bool             set       (unsigned index, uint64_t)                    noexcept;
    bool             set       (unsigned index, int)                         noexcept;
    bool             set       (unsigned index, unsigned)                    noexcept;
    bool             set       (unsigned index, Object&&)                    noexcept;
    bool             set       (unsigned index, Array&&)                     noexcept;
    bool             set       (unsigned index, std::nullptr_t = nullptr)    noexcept;
    bool             set       (unsigned index, bool)                        noexcept;
    bool             push      (String&&)                              noexcept;
    bool             push      (double)                                noexcept;
    bool             push      (int64_t)                               noexcept;
    bool             push      (uint64_t)                              noexcept;
    bool             push      (int)                                   noexcept;
    bool             push      (unsigned)                              noexcept;
    bool             push      (Object&&)                              noexcept;
    bool             push      (Array&&)                               noexcept;
    bool             push      (std::nullptr_t = nullptr)              noexcept;
    bool             push      (bool)                                  noexcept;
    ValueType       *back      ()                                      noexcept;
    ValueType       &unsafeBack()                                      noexcept;
    unsigned         size      () const                                noexcept;

    ValueType       *operator[](unsigned)       noexcept;
    const ValueType *operator[](unsigned) const noexcept;

    Allocator getAllocator() const noexcept;

    iterator       begin()       noexcept;
    const_iterator begin() const noexcept;
    iterator       end()         noexcept;
    const_iterator end()   const noexcept;

private:
    Container m_data;
};



}