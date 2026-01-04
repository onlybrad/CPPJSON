#pragma once

#include <unordered_map>
#include <cstdint>

#include "allocator.hpp"
#include "string.hpp"

namespace CPPJSON {

class Parser;
class JSON;
class Array;

class Object {
    
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
    ~Object();
    Object &operator=(const Object&);
    Object &operator=(Object&&)      noexcept;

    bool           reserve   (unsigned capacity = 0U)                       noexcept;
    ValueType     *get       (const std::string&)                           noexcept;
    ValueType     *get       (const String&)                                noexcept;
    ValueType     *get       (const char *const)                            noexcept;
    void           remove    (const std::string&)                           noexcept;
    void           remove    (const String&)                                noexcept;
    void           remove    (const char *const)                            noexcept;
    String        *getString (const std::string&, bool &success)            noexcept;
    double         getFloat64(const std::string&, bool &success)            noexcept;
    std::int64_t   getInt64  (const std::string&, bool &success)            noexcept;
    std::uint64_t  getUint64 (const std::string&, bool &success)            noexcept;
    Object        *getObject (const std::string&, bool &success)            noexcept;
    Array         *getArray  (const std::string&, bool &success)            noexcept;
    std::nullptr_t getNull   (const std::string&, bool &success)            noexcept;
    bool           getBool   (const std::string&, bool &success)            noexcept;
    String        *getString (const String&, bool &success)                 noexcept;
    double         getFloat64(const String&, bool &success)                 noexcept;
    std::int64_t   getInt64  (const String&, bool &success)                 noexcept;
    std::uint64_t  getUint64 (const String&, bool &success)                 noexcept;
    Object        *getObject (const String&, bool &success)                 noexcept;
    Array         *getArray  (const String&, bool &success)                 noexcept;
    std::nullptr_t getNull   (const String&, bool &success)                 noexcept;
    bool           getBool   (const String&, bool &success)                 noexcept;
    String        *getString (const char*, bool &success)                   noexcept;
    double         getFloat64(const char*, bool &success)                   noexcept;
    std::int64_t   getInt64  (const char*, bool &success)                   noexcept;
    std::uint64_t  getUint64 (const char*, bool &success)                   noexcept;
    Object        *getObject (const char*, bool &success)                   noexcept;
    Array         *getArray  (const char*, bool &success)                   noexcept;
    std::nullptr_t getNull   (const char*, bool &success)                   noexcept;
    bool           getBool   (const char*, bool &success)                   noexcept;
    String        *getString (const std::string&)                           noexcept;
    double         getFloat64(const std::string&)                           noexcept;
    std::int64_t   getInt64  (const std::string&)                           noexcept;
    std::uint64_t  getUint64 (const std::string&)                           noexcept;
    Object        *getObject (const std::string&)                           noexcept;
    Array         *getArray  (const std::string&)                           noexcept;
    std::nullptr_t getNull   (const std::string&)                           noexcept;
    bool           getBool   (const std::string&)                           noexcept;
    String        *getString (const String&)                                noexcept;
    double         getFloat64(const String&)                                noexcept;
    std::int64_t   getInt64  (const String&)                                noexcept;
    std::uint64_t  getUint64 (const String&)                                noexcept;
    Object        *getObject (const String&)                                noexcept;
    Array         *getArray  (const String&)                                noexcept;
    std::nullptr_t getNull   (const String&)                                noexcept;
    bool           getBool   (const String&)                                noexcept;
    String        *getString (const char*)                                  noexcept;
    double         getFloat64(const char*)                                  noexcept;
    std::int64_t   getInt64  (const char*)                                  noexcept;
    std::uint64_t  getUint64 (const char*)                                  noexcept;
    Object        *getObject (const char*)                                  noexcept;
    Array         *getArray  (const char*)                                  noexcept;
    std::nullptr_t getNull   (const char*)                                  noexcept;
    bool           getBool   (const char*)                                  noexcept;
    bool           set       (const std::string&, JSON&&)                   noexcept;
    bool           set       (const std::string&, String&&)                 noexcept;
    bool           set       (const std::string&, double)                   noexcept;
    bool           set       (const std::string&, int64_t)                  noexcept;
    bool           set       (const std::string&, uint64_t)                 noexcept;
    bool           set       (const std::string&, int)                      noexcept;
    bool           set       (const std::string&, unsigned)                 noexcept;
    bool           set       (const std::string&, Object&&)                 noexcept;
    bool           set       (const std::string&, Array&&)                  noexcept;
    bool           set       (const std::string&, std::nullptr_t = nullptr) noexcept;
    bool           set       (const std::string&, bool)                     noexcept;
    bool           set       (const String&, JSON&&)                        noexcept;
    bool           set       (const String&, String&&)                      noexcept;
    bool           set       (const String&, double)                        noexcept;
    bool           set       (const String&, int64_t)                       noexcept;
    bool           set       (const String&, uint64_t)                      noexcept;
    bool           set       (const String&, int)                           noexcept;
    bool           set       (const String&, unsigned)                      noexcept;
    bool           set       (const String&, Object&&)                      noexcept;
    bool           set       (const String&, Array&&)                       noexcept;
    bool           set       (const String&, std::nullptr_t = nullptr)      noexcept;
    bool           set       (const String&, bool)                          noexcept;
    bool           set       (const char*, JSON&&)                          noexcept;
    bool           set       (const char*, String&&)                        noexcept;
    bool           set       (const char*, double)                          noexcept;
    bool           set       (const char*, int64_t)                         noexcept;
    bool           set       (const char*, uint64_t)                        noexcept;
    bool           set       (const char*, int)                             noexcept;
    bool           set       (const char*, unsigned)                        noexcept;
    bool           set       (const char*, Object&&)                        noexcept;
    bool           set       (const char*, Array&&)                         noexcept;
    bool           set       (const char*, std::nullptr_t = nullptr)        noexcept;
    bool           set       (const char*, bool)                            noexcept;

    ValueType *operator[](const std::string&)  noexcept;
    ValueType *operator[](const String&)       noexcept;
    ValueType *operator[](String&&)            noexcept;
    ValueType *operator[](String::Container&&) noexcept;
    ValueType *operator[](const char*)         noexcept;

    Allocator getAllocator() const noexcept;
    
private:
    Container m_data;
};

}