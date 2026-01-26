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

    typedef Container::const_iterator const_iterator;

    static const unsigned MINIMUM_CAPACITY; 

    String(const Allocator&);
    String(Allocator&&)       noexcept;
    String(String&&)          noexcept = default;
    String(const String&);
    ~String()                 noexcept;

    ValueType       &operator[](unsigned)                  noexcept;
    const ValueType &operator[](unsigned)            const noexcept;
    const String    &operator+=(const std::string&);
    const String    &operator= (const std::string&);
    bool             operator==(const std::string&)  const;
    String &         operator= (const String&);
    String &         operator= (String&&)                   noexcept;
    const String    &operator+=(const String&);
    bool             operator==(const String&)       const;
    const String    &operator+=(const char*);
    const String    &operator= (const char*);
    bool             operator==(const char*)         const;
    unsigned         size      ()                    const noexcept;
    bool             reserve   (unsigned);
    void             push      (ValueType);

    const String &operator+=(std::nullptr_t) = delete;
    const String &operator= (std::nullptr_t) = delete;

    unsigned toStringSize()              const noexcept;
    void     toString    (std::string&) const noexcept;

    const char       *getCString  () const noexcept;
    Allocator         getAllocator() const noexcept;   

    void destructor() noexcept;

    const_iterator begin() const noexcept;
    const_iterator end()   const noexcept;

private:
    Container m_data;
};

}

