#include <cstring>

#include "json.hpp"

namespace CPPJSON {

const JSON JSON::INVALID_JSON = {};

template<>
Result<double> JSON::asValue<double, JSON::Type::FLOAT64, nullptr>() const noexcept {
    switch(m_type) {    
    case Type::FLOAT64:
        return Result<double>::fromValue(m_value.float64);

    case Type::INT64:
        return Result<double>::fromValue(static_cast<double>(m_value.int64));
    
    case Type::UINT64:
        return Result<double>::fromValue(static_cast<double>(m_value.uint64));

    default:
        return Result<double>::fromError(true);
    }
}

template<>
Result<std::int64_t> JSON::asValue<std::int64_t, JSON::Type::INT64, nullptr>() const noexcept {
    constexpr double minInt64d = static_cast<double>(std::numeric_limits<std::int64_t>().min());
    constexpr double maxInt64d = static_cast<double>(std::numeric_limits<std::int64_t>().max());
    constexpr std::uint64_t maxInt64u = static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>().max());

    switch(m_type) {    
    case Type::FLOAT64:
        return m_value.float64 < minInt64d || m_value.float64 > maxInt64d 
            ? Result<std::int64_t>::fromError(true)
            : Result<std::int64_t>::fromValue(static_cast<std::int64_t>(m_value.float64));

    case Type::INT64:
        return Result<std::int64_t>::fromValue(m_value.int64);
    
    case Type::UINT64:
        return m_value.uint64 > maxInt64u
            ? Result<std::int64_t>::fromError(true)
            : Result<std::int64_t>::fromValue(static_cast<std::int64_t>(m_value.uint64));

    default:
        return Result<std::int64_t>::fromError(true);
    }
}

template<>
Result<std::uint64_t> JSON::asValue<std::uint64_t, JSON::Type::UINT64, nullptr>() const noexcept {
    constexpr double maxUint64 = static_cast<double>(std::numeric_limits<std::uint64_t>().max());

    switch(m_type) {
    case Type::FLOAT64:
        return m_value.float64 < 0.0 || m_value.float64 > maxUint64
            ? Result<std::uint64_t>::fromError(true)
            : Result<std::uint64_t>::fromValue(static_cast<std::uint64_t>(m_value.float64));

    case Type::INT64:
        return m_value.int64 < 0
            ? Result<std::uint64_t>::fromError(true)
            : Result<std::uint64_t>::fromValue(static_cast<std::uint64_t>(m_value.int64));
    
    case Type::UINT64:
        return Result<std::uint64_t>::fromValue(m_value.uint64);

    default:
        return Result<std::uint64_t>::fromError(true);
    }
}

JSON::JSON(const JSON &json) {
    copy(json);
}

JSON::JSON(JSON &&json) noexcept : 
m_type(json.m_type) {
    std::memcpy(static_cast<void*>(&m_value), &json.m_value, sizeof(m_value));
    json.set();
}

JSON::JSON(const double value) noexcept :
m_type(Type::FLOAT64),
m_value(value)
{}

JSON::JSON(const std::int64_t value) noexcept :
m_type(Type::INT64),
m_value(value)
{}

JSON::JSON(const std::uint64_t value) noexcept : 
m_type(Type::UINT64),
m_value(value)
{}

JSON::JSON(const int value) noexcept :
JSON(static_cast<int64_t>(value))
{}

JSON::JSON(const unsigned value) noexcept :
JSON(static_cast<uint64_t>(value))
{}

JSON::JSON(const bool value) noexcept : 
m_type(Type::BOOL),
m_value(value) 
{}

JSON::JSON(std::nullptr_t) noexcept :
m_type(Type::NUL),
m_value(nullptr)
{}

JSON::~JSON() noexcept {
    destructor();
}

JSON &JSON::operator=(const JSON &json) {
    destructor();
    copy(json);

    return *this;
}

JSON &JSON::operator=(JSON &&json) noexcept {
    return set(std::move(json));
}

bool JSON::operator==(const JSON &json) const noexcept {
    if(this == &json) {
        return true;
    }

    if(isNumber() && json.isNumber() && m_type != json.m_type) {
        if(m_type == Type::FLOAT64 || json.m_type == Type::FLOAT64) {
            return unsafeAsFloat64() == json.unsafeAsFloat64();
        }

        std::int64_t  intValue;
        std::uint64_t uintValue;
        if(m_type == Type::INT64) {
            intValue  = m_value.int64;
            uintValue = json.m_value.uint64;
        } else {
            intValue  = json.m_value.int64;
            uintValue = m_value.uint64;
        }

        return intValue < 0
            ? false
            : static_cast<std::uint64_t>(intValue) == uintValue;
    }

    if(m_type != json.m_type) {
        return false;
    }

    switch(m_type) {
    case Type::STRING: 
        return m_value.string == json.m_value.string;

    case Type::FLOAT64:
        return m_value.float64 == json.m_value.float64;

    case Type::INT64:
        return m_value.int64 == json.m_value.int64;

    case Type::UINT64:
        return m_value.uint64 == json.m_value.uint64;
    
    case Type::ARRAY:
        if(m_value.array.size() != json.m_value.array.size()) {
            return false;
        }

        for(unsigned i = 0U; i < m_value.array.size(); i++) {
            if(m_value.array[i] != json.m_value.array[i]) {
                return false;
            }
        }

        return true;

    case Type::OBJECT:
        if(m_value.object.size() != json.m_value.object.size()) {
            return false;
        }

        for(const Object::KeyValueType &keyValue : m_value.object) {
            if(keyValue.second != json.m_value.object[keyValue.first]) {
                return false;
            }
        }

        return true;

    case Type::NUL:
        return true;

    case Type::BOOL:
        return m_value.boolean == json.m_value.boolean;
    }

    return false;
}

bool JSON::operator!=(const JSON &json) const noexcept {
    return !((*this) == json);
}

JSON::Type JSON::getType() const noexcept {
    return m_type;
}

bool JSON::isNumber() const noexcept {
    return m_type == Type::FLOAT64 || m_type == Type::INT64 || m_type == Type::UINT64;
}

bool JSON::isValid() const noexcept {
    return this != &INVALID_JSON;
}

QueryBuilder<false> JSON::operator[](const unsigned index) noexcept { 
    QueryBuilder<false> queryResult = QueryBuilder<false>(this); 
    queryResult[index];
    return queryResult;
}

QueryBuilder<false> JSON::operator[](const std::string &key) noexcept {
    return (*this)[key.c_str()];
}

QueryBuilder<false> JSON::operator[](const String &key) noexcept { 
    QueryBuilder<false> queryResult = QueryBuilder<false>(this); 
    queryResult[key];
    return queryResult;
}

QueryBuilder<false> JSON::operator[](const char *const key) noexcept {
    assert(key != nullptr);

    String stringKey(Object::getKeyAllocator());
    stringKey = key;
    const QueryBuilder<false> ret = (*this)[stringKey];
    Object::getKeyAllocator().reset();
    
    return ret;
}

QueryBuilder<true> JSON::operator[](const unsigned index) const noexcept { 
    QueryBuilder<true> queryResult = QueryBuilder<true>(this); 
    queryResult[index];
    return queryResult;
}

QueryBuilder<true> JSON::operator[](const std::string &key) const noexcept {
    return (*this)[key.c_str()];
}

QueryBuilder<true> JSON::operator[](const String &key) const noexcept { 
    QueryBuilder<true> queryResult = QueryBuilder<true>(this); 
    queryResult[key];
    return queryResult;
}

QueryBuilder<true> JSON::operator[](const char *const key) const noexcept {
    String stringKey(Object::getKeyAllocator());
    stringKey = key;
    const QueryBuilder<true> ret = (*this)[stringKey];
    Object::getKeyAllocator().reset();

    return ret;
}

Result<String&> JSON::asString() noexcept { 
    return asRef<String, Type::STRING, &Value::string>();
}

Result<Object&> JSON::asObject() noexcept { 
    return asRef<Object, Type::OBJECT, &Value::object>();
}

Result<Array&> JSON::asArray() noexcept { 
    return asRef<Array, Type::ARRAY, &Value::array>();
}

Result<const String&> JSON::asString() const noexcept { 
    return asConstRef<String, Type::STRING, &Value::string>();
}

Result<double> JSON::asFloat64() const noexcept { 
    return asValue<double, Type::FLOAT64, nullptr>();
}

Result<std::int64_t> JSON::asInt64() const noexcept { 
    return asValue<std::int64_t, Type::INT64, nullptr>();
}

Result<std::uint64_t> JSON::asUint64() const noexcept { 
    return asValue<std::uint64_t, Type::UINT64, nullptr>();
}

Result<const Object&> JSON::asObject() const noexcept { 
    return asConstRef<Object, Type::OBJECT, &Value::object>();
}

Result<const Array&> JSON::asArray() const noexcept { 
    return asConstRef<Array, Type::ARRAY, &Value::array>();
}

Result<std::nullptr_t> JSON::asNull() const noexcept { 
    return asValue<std::nullptr_t, Type::NUL, &Value::null>();
}

Result<bool> JSON::asBool() const noexcept {
    return asValue<bool, Type::BOOL, &Value::boolean>();
}

String &JSON::unsafeAsString() noexcept {
    return m_value.string;
}

Object &JSON::unsafeAsObject() noexcept {
    return m_value.object;
}

Array &JSON::unsafeAsArray() noexcept {
    return m_value.array;
}

const String &JSON::unsafeAsString() const noexcept {
    return m_value.string;
}

double JSON::unsafeAsFloat64() const noexcept {
    switch(m_type) {
    case Type::FLOAT64:
    default:
        return m_value.float64;

    case Type::INT64:
        return static_cast<double>(m_value.int64);

    case Type::UINT64:
        return static_cast<double>(m_value.uint64);
    }
}

int64_t JSON::unsafeAsInt64() const noexcept {
    switch(m_type) {
    case Type::FLOAT64:
        return static_cast<std::int64_t>(m_value.float64);

    case Type::INT64:
    default:
        return m_value.int64;

    case Type::UINT64:
        return static_cast<std::int64_t>(m_value.uint64);
    }
}

uint64_t JSON::unsafeAsUint64() const noexcept {
    switch(m_type) {
    case Type::FLOAT64:
        return static_cast<std::uint64_t>(m_value.float64);

    case Type::INT64:
        return static_cast<std::uint64_t>(m_value.int64);

    case Type::UINT64:
    default:
        return m_value.uint64;
    }
}

const Object &JSON::unsafeAsObject() const noexcept {
    return m_value.object;
}

const Array &JSON::unsafeAsArray() const noexcept {
    return m_value.array;
}

std::nullptr_t JSON::unsafeAsNull() const noexcept {
    return m_value.null;
}

bool JSON::unsafeAsBool() const noexcept {
    return m_value.boolean;
}

Result<Object&> JSON::makeObject(const Object::Allocator &allocator) noexcept {
    destructor();
    m_type = Type::OBJECT;

    try {
        new (&m_value.object) Object(allocator);
        return Result<Object&>::fromRef(m_value.object);
    } catch(...) {
        return Result<Object&>::fromError(true);
    }
}

Result<Array&> JSON::makeArray(const Array::Allocator &allocator) noexcept {
    destructor();
    m_type = Type::ARRAY;

    try {
        new (&m_value.array) Array(allocator);
        return Result<Array&>::fromRef(m_value.array);
    } catch(...) {
        return Result<Array&>::fromError(true);
    }
}

Result<String&> JSON::makeString(const String::Allocator &allocator) noexcept {
    destructor();
    m_type = Type::STRING;

    try {
        new (&m_value.string) String(allocator);
        return Result<String&>::fromRef(m_value.string);
    } catch(...) {
        return Result<String&>::fromError(true);
    }
}

void JSON::destructor() noexcept {
    switch(m_type) {
    case Type::STRING:
        m_value.string.destructor();
        break;

    case Type::ARRAY:
        m_value.array.destructor();
        break;

    case Type::OBJECT:
        m_value.object.destructor();
        break;

    default:;
    }
}

void JSON::copy(const JSON &json) {
    m_type = json.m_type;

    switch(m_type) {
    case Type::STRING:
        m_value = json.m_value.string;
        break;

    case Type::ARRAY:
        m_value = json.m_value.array;
        break;

    case Type::OBJECT:
        m_value = json.m_value.object;
        break;
        
    default:
        std::memcpy(static_cast<void*>(&m_value), &json.m_value, sizeof(m_value));
        break;
    }    
}

JSON &JSON::set(const std::string &value, const String::Allocator &allocator) {
    return set(value.c_str(), allocator);
}

JSON &JSON::set(const char *const value, const String::Allocator &allocator) {
    assert(value != nullptr);

    if(m_type != Type::STRING) {
        destructor();
        new (&m_value.string) String(allocator);
        m_type = Type::STRING;
    }
    m_value.string = value;

    return *this;
}

JSON &JSON::set(const String &value) {
    return set(value.getCString(), value.getAllocator());
}

JSON &JSON::set(JSON &&value) {
    if (this != &value) {
        destructor();
        m_type = value.m_type;
        std::memcpy(static_cast<void*>(&m_value), &value.m_value, sizeof(m_value));
        value.set();
    }

    return *this;
}

JSON &JSON::set(String &&value) {
    if(m_type != Type::STRING) {
        destructor();
        new (&m_value.string) String(value.getAllocator());
        m_type = Type::STRING;
    }
    m_value.string = std::move(value);
    
    return *this;
}

JSON &JSON::set(const double value) noexcept {
    destructor();
    m_type  = Type::FLOAT64;
    m_value = value;

    return *this;
}

JSON &JSON::set(const int64_t value) noexcept {
    destructor();
    m_type  = Type::INT64;
    m_value = value;

    return *this;
}

JSON &JSON::set(const uint64_t value) noexcept {
    destructor();
    m_type  = Type::UINT64;
    m_value = value;

    return *this;
}

JSON &JSON::set(const int value) noexcept {
    return set(static_cast<int64_t>(value));
}

JSON &JSON::set(const unsigned value) noexcept {
    return set(static_cast<uint64_t>(value));
}

JSON &JSON::set(Object &&object) {
    if(m_type != Type::OBJECT) {
        destructor();
        m_type = Type::OBJECT;
        new (&m_value.object) Object(object.getAllocator());
    }
    m_value.object = std::move(object);
    
    return *this;
}

JSON &JSON::set(Array &&array) {
    if(m_type != Type::ARRAY) {
        destructor();
        m_type = Type::ARRAY;
        new (&m_value.array) Array(array.getAllocator());
    }
    m_value.array = std::move(array);
    
    return *this;
}

JSON &JSON::set(const std::nullptr_t) noexcept {
    destructor();
    m_type       = Type::NUL;
    m_value.null = nullptr;

    return *this;
}

JSON &JSON::set(const bool value) noexcept {
    destructor();
    m_type          = Type::BOOL;
    m_value.boolean = value;

    return *this;
}

JSON &JSON::operator=(const String &value) {
    return set(value);
} 

JSON &JSON::operator=(String &&value) {
    return set(std::move(value));
}

JSON &JSON::operator=(const double value) noexcept {
    return set(value);
}

JSON &JSON::operator=(const int64_t value) noexcept {
    return set(value);
}

JSON &JSON::operator=(const uint64_t value) noexcept {
    return set(value);
}

JSON &JSON::operator=(const int value) noexcept {
    return set(value);
}

JSON &JSON::operator=(const unsigned value) noexcept {
    return set(value);
}

JSON &JSON::operator=(Object&& value) {
    return set(std::move(value));
}

JSON &JSON::operator=(Array&& value) {
    return set(std::move(value));
}

JSON &JSON::operator=(const std::nullptr_t value) noexcept {
    return set(value);
}

JSON &JSON::operator=(const bool value) noexcept {
    return set(value);
}

JSON::Value::Value(const double value) noexcept :
float64(value)
{}

JSON::Value::Value(const std::int64_t value) noexcept :
int64(value)
{}

JSON::Value::Value(const std::uint64_t value) noexcept :
uint64(value)
{}

JSON::Value::Value(std::nullptr_t) noexcept :
null()
{}

JSON::Value::Value(const bool value) noexcept :
boolean(value)
{}

JSON::Value::~Value() noexcept {}

JSON::Value &JSON::Value::operator=(const String &value) {
    string = value;
    return *this;
}

JSON::Value &JSON::Value::operator=(String &&value) noexcept {
    string = std::move(value);
    return *this;
}

JSON::Value &JSON::Value::operator=(const double value) noexcept {
    float64 = value;
    return *this;
}

JSON::Value &JSON::Value::operator=(const std::int64_t value) noexcept {
    int64 = value;
    return *this;
}

JSON::Value &JSON::Value::operator=(const std::uint64_t value) noexcept {
    uint64 = value;
    return *this;
}

JSON::Value &JSON::Value::operator=(const Object &value) {
    object = value;
    return *this;
}

JSON::Value &JSON::Value::operator=(Object &&value) noexcept {
    object = std::move(value);
    return *this;
}

JSON::Value &JSON::Value::operator=(const Array &value) {
    array = value;
    return *this;
}

JSON::Value &JSON::Value::operator=(Array &&value) noexcept {
    array = std::move(value);
    return *this;
}

JSON::Value &JSON::Value::operator=(const std::nullptr_t) noexcept {
    null = nullptr;
    return *this;
}

JSON::Value &JSON::Value::operator=(const bool value) noexcept {
    boolean = value;
    return *this;
}

}