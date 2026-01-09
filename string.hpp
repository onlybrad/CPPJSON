#pragma once

#include <string>

#include "allocator.hpp"

class Root;

namespace CPPJSON {

class String {

public:
    typedef char                          ValueType;
    typedef ValueType                     ContainerType;
    typedef ArenaAllocator<ContainerType> Allocator;
    typedef std::basic_string<
        ContainerType, 
        std::char_traits<ContainerType>,
        Allocator
    > Container;

    static const unsigned MINIMUM_CAPACITY; 

    String(const Allocator&);
    String(Allocator&&)       noexcept;
    String(String&&)          noexcept = default;
    ~String()                 noexcept;

    ValueType           &operator[](unsigned)                   noexcept;
    const ValueType     &operator[](unsigned)             const noexcept;
    const String        &operator+=(const std::string&);
    const String        &operator= (const std::string&);
    bool                 operator==(const std::string&)   const;
    String&              operator= (const String&);
    String&              operator= (String&&)                   noexcept;
    const String        &operator+=(const String&);
    bool                 operator==(const String&)        const;
    const String        &operator+=(const char*);
    const String        &operator= (const char*);
    bool                 operator==(const char*)          const;
    unsigned             size      ()                     const noexcept;
    bool                 reserve   (unsigned);
    void                 push      (ValueType);

    const String &operator+=(std::nullptr_t) = delete;
    const String &operator= (std::nullptr_t) = delete;

    const char *getCStr     () const noexcept;
    Container  &getContainer()       noexcept;
    Allocator   getAllocator() const noexcept;   

private:
    Container m_data;
};

}

