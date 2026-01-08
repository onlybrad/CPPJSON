#include <cstring>

#include "json.hpp"

namespace CPPJSON {

QueryBuilder::QueryBuilder(JSON *const json) noexcept :
m_json(json)
{}

QueryBuilder &QueryBuilder::operator[](const unsigned index) noexcept {
    if(m_json == nullptr) {
        return *this;
    }

    if(m_json->m_type != JSON::Type::ARRAY) {
        m_json = nullptr;
        return *this;
    }

    m_json = m_json->m_value.array.get(index);
    return *this;
}

QueryBuilder &QueryBuilder::operator[](const std::string &key) noexcept {
    return this->operator[](key.c_str());
}

QueryBuilder &QueryBuilder::operator[](const String &key) noexcept {
    return this->operator[](key.getData());
}

QueryBuilder &QueryBuilder::operator[](const char *const key) noexcept {
    assert(key != nullptr);

    if(m_json == nullptr) {
        return *this;
    }

    if(m_json->m_type != JSON::Type::OBJECT) {
        m_json = nullptr;
        return *this;
    }

    m_json = m_json->m_value.object.get(key);
    return *this;
}

JSON *QueryBuilder::get() noexcept { 
    return m_json; 
}

String *QueryBuilder::asString(bool &success) noexcept { 
    return asPtr<String, &JSON::asString>(success);
}

double QueryBuilder::asFloat64(bool &success) const noexcept { 
    return as<double, &JSON::asFloat64>(success);
}

int64_t QueryBuilder::asInt64(bool &success) const noexcept { 
    return as<std::int64_t, &JSON::asInt64>(success);
}

uint64_t QueryBuilder::asUint64(bool &success) const noexcept { 
    return as<std::uint64_t, &JSON::asUint64>(success);
}

Object *QueryBuilder::asObject(bool &success) noexcept { 
    return asPtr<Object, &JSON::asObject>(success);
}

Array *QueryBuilder::asArray(bool &success) noexcept { 
    return asPtr<Array, &JSON::asArray>(success);
}

std::nullptr_t QueryBuilder::asNull(bool &success) const noexcept { 
    return as<std::nullptr_t, &JSON::asNull>(success);
}

bool QueryBuilder::asBool(bool &success) const noexcept { 
    return as<bool, &JSON::asBool>(success);
}

String *QueryBuilder::asString() noexcept {
    bool success;
    return asString(success);
}

double QueryBuilder::asFloat64() const noexcept {
    bool success;
    return asFloat64(success);
}

int64_t QueryBuilder::asInt64() const noexcept {
    bool success;
    return asInt64(success);
}

uint64_t QueryBuilder::asUint64() const noexcept {
    bool success;
    return asUint64(success);
}

Object *QueryBuilder::asObject() noexcept {
    bool success;
    return asObject(success);
}

Array *QueryBuilder::asArray() noexcept {
    bool success;
    return asArray(success);
}

std::nullptr_t QueryBuilder::asNull() const noexcept {
    bool success;
    return asNull(success);
}

bool QueryBuilder::asBool() const noexcept {
    bool success;
    return asBool(success);
}

bool QueryBuilder::asString(AsCallbackRef<String> callback, FailureCallback failureCallback) noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<String, &JSON::asString>(callback, failureCallback);
}

bool QueryBuilder::asFloat64(AsCallback<double> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<double, &JSON::asFloat64>(callback, failureCallback);
}

bool QueryBuilder::asInt64(AsCallback<std::int64_t> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<std::int64_t, &JSON::asInt64>(callback, failureCallback);
}

bool QueryBuilder::asUint64(AsCallback<std::uint64_t> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<std::uint64_t, &JSON::asUint64>(callback, failureCallback);
}

bool QueryBuilder::asObject(AsCallbackRef<Object> callback, FailureCallback failureCallback) noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<Object, &JSON::asObject>(callback, failureCallback);
}

bool QueryBuilder::asArray(AsCallbackRef<Array> callback, FailureCallback failureCallback) noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<Array, &JSON::asArray>(callback, failureCallback);
}

bool QueryBuilder::asNull(AsCallback<std::nullptr_t> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<std::nullptr_t, &JSON::asNull>(callback, failureCallback);
}

bool QueryBuilder::asBool(AsCallback<bool> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<bool, &JSON::asBool>(callback, failureCallback);
}

bool QueryBuilder::asString(AsCallbackRef<String> callback) noexcept {
    assert(callback != nullptr);

    return as<String, &JSON::asString>(callback);
}

bool QueryBuilder::asFloat64(AsCallback<double> callback) const noexcept {
    assert(callback != nullptr);

    return as<double, &JSON::asFloat64>(callback);
}

bool QueryBuilder::asInt64(AsCallback<std::int64_t> callback) const noexcept {
    assert(callback != nullptr);

    return as<std::int64_t, &JSON::asInt64>(callback);
}

bool QueryBuilder::asUint64(AsCallback<std::uint64_t> callback) const noexcept {
    assert(callback != nullptr);

    return as<std::uint64_t, &JSON::asUint64>(callback);
}

bool QueryBuilder::asObject(AsCallbackRef<Object> callback) noexcept {
    assert(callback != nullptr);

    return as<Object, &JSON::asObject>(callback);
}

bool QueryBuilder::asArray(AsCallbackRef<Array> callback) noexcept {
    assert(callback != nullptr);

    return as<Array, &JSON::asArray>(callback);
}

bool QueryBuilder::asNull(AsCallback<std::nullptr_t> callback) const noexcept {
    assert(callback != nullptr);

    return as<std::nullptr_t, &JSON::asNull>(callback);
}

bool QueryBuilder::asBool(AsCallback<bool> callback) const noexcept {
    assert(callback != nullptr);
    
    return as<bool, &JSON::asBool>(callback);
}

template<>
std::int64_t JSON::as<std::int64_t, JSON::Type::INT64, nullptr>(bool &success) const noexcept {
    if(m_type == Type::UINT64) {
        if(m_value.uint64 > std::numeric_limits<std::int64_t>().max()) {
            success = false;
            return 0;
        }
        success = true;
        return static_cast<std::int64_t>(m_value.uint64);
    }

    if(m_type != Type::INT64) {
        success = false;
        return 0;
    }

    success = true;
    return m_value.int64;
}

template<>
std::uint64_t JSON::as<std::uint64_t, JSON::Type::UINT64, nullptr>(bool &success) const noexcept {
    if(m_type == Type::INT64) {
        if(m_value.int64 < 0) {
            success = false;
            return 0;
        }
        success = true;
        return static_cast<std::uint64_t>(m_value.int64);
    }

    if(m_type != Type::UINT64) {
        success = false;
        return 0;
    }

    success = true;
    return m_value.uint64;
}

String *JSON::asString() noexcept {
    bool success;
    return asString(success);
}

double JSON::asFloat64() noexcept {
    bool success;
    return asFloat64(success);
}

int64_t JSON::asInt64() const noexcept {
    bool success;
    return asInt64(success);
};

uint64_t JSON::asUint64() const noexcept {
    bool success;
    return asUint64(success);
};

Object *JSON::asObject() noexcept {
    bool success;
    return asObject(success);
};

Array *JSON::asArray () noexcept {
    bool success;
    return asArray(success);
};

std::nullptr_t JSON::asNull() const noexcept {
    bool success;
    return asNull(success);
};

bool JSON::asBool() const noexcept {
    bool success;
    return asBool(success);
};

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
    if (this != &json) {
        destructor();
        m_type = json.m_type;
        std::memcpy(static_cast<void*>(&m_value), &json.m_value, sizeof(m_value));
        json.set();
    }

    return *this;
}

JSON::Type JSON::getType() const noexcept {
    return m_type;
}

JSON::Error JSON::getError() const noexcept {
    if(m_type != Type::ERROR) {
        return Error::NONE;
    }

    return m_value.error;
}

QueryBuilder JSON::operator[](const unsigned index) noexcept { 
    QueryBuilder queryResult = QueryBuilder(this); 
    queryResult[index];
    return queryResult;
}

QueryBuilder JSON::operator[](const std::string &key) noexcept { 
    QueryBuilder queryResult = QueryBuilder(this); 
    queryResult[key];
    return queryResult;
}

QueryBuilder JSON::operator[](const String &key) noexcept { 
    QueryBuilder queryResult = QueryBuilder(this); 
    queryResult[key];
    return queryResult;
}

QueryBuilder JSON::operator[](const char *const key) noexcept {
    assert(key != nullptr);
    
    QueryBuilder queryResult = QueryBuilder(this); 
    queryResult[key];
    return queryResult;
}

String *JSON::asString(bool &success) noexcept { 
    return asPtr<String, Type::STRING, &JSON::Value::string>(success);
}

double JSON::asFloat64(bool &success) const noexcept { 
    return as<double, Type::FLOAT64, &JSON::Value::float64>(success);
}

std::int64_t JSON::asInt64(bool &success) const noexcept { 
    return as<std::int64_t, Type::INT64, nullptr>(success);
}

std::uint64_t JSON::asUint64(bool &success) const noexcept { 
    return as<std::uint64_t, Type::UINT64, nullptr>(success);
}

Object *JSON::asObject(bool &success) noexcept { 
    return asPtr<Object, Type::OBJECT, &JSON::Value::object>(success);
}

Array *JSON::asArray(bool &success) noexcept { 
    return asPtr<Array, Type::ARRAY, &JSON::Value::array>(success);
}

std::nullptr_t JSON::asNull(bool &success) const noexcept { 
    return as<std::nullptr_t, Type::NUL, &JSON::Value::null>(success);
}

bool JSON::asBool(bool &success) const noexcept {
    return as<bool, Type::BOOL, &JSON::Value::boolean>(success);
}

bool JSON::asString(AsCallbackRef<String> callback, FailureCallback failureCallback) noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<String, Type::STRING, &Value::string>(callback, failureCallback);
}

bool JSON::asFloat64(AsCallback<double> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<double, Type::FLOAT64, &Value::float64>(callback, failureCallback);
}

bool JSON::asInt64(AsCallback<std::int64_t> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<std::int64_t, Type::INT64, nullptr>(callback, failureCallback);
}

bool JSON::asUint64(AsCallback<std::uint64_t> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<std::uint64_t, Type::UINT64, nullptr>(callback, failureCallback);
}

bool JSON::asObject(AsCallbackRef<Object> callback, FailureCallback failureCallback) noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<Object, Type::OBJECT, &Value::object>(callback, failureCallback);
}

bool JSON::asArray(AsCallbackRef<Array> callback, FailureCallback failureCallback) noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<Array, Type::ARRAY, &Value::array>(callback, failureCallback);
}

bool JSON::asNull(AsCallback<std::nullptr_t> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<std::nullptr_t, Type::NUL, &Value::null>(callback, failureCallback);
}

bool JSON::asBool(AsCallback<bool> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return as<bool, Type::BOOL, &Value::boolean>(callback, failureCallback);
}

bool JSON::asString(AsCallbackRef<String> callback) noexcept {
    assert(callback != nullptr);

    return as<String, Type::STRING, &Value::string>(callback);
}

bool JSON::asFloat64(AsCallback<double> callback) const noexcept {
    assert(callback != nullptr);

    return as<double, Type::FLOAT64, &Value::float64>(callback);
}

bool JSON::asInt64(AsCallback<std::int64_t> callback) const noexcept {
    assert(callback != nullptr);

    return as<std::int64_t, Type::INT64, nullptr>(callback);
}

bool JSON::asUint64(AsCallback<std::uint64_t> callback) const noexcept {
    assert(callback != nullptr);

    return as<std::uint64_t, Type::UINT64, nullptr>(callback);
}

bool JSON::asObject(AsCallbackRef<Object> callback) noexcept {
    assert(callback != nullptr);

    return as<Object, Type::OBJECT, &Value::object>(callback);
}

bool JSON::asArray(AsCallbackRef<Array> callback) noexcept {
    assert(callback != nullptr);

    return as<Array, Type::ARRAY, &Value::array>(callback);
}

bool JSON::asNull(AsCallback<std::nullptr_t> callback) const noexcept {
    assert(callback != nullptr);

    return as<std::nullptr_t, Type::NUL, &Value::null>(callback);
}

bool JSON::asBool(AsCallback<bool> callback) const noexcept {
    assert(callback != nullptr);

    return as<bool, Type::BOOL, &Value::boolean>(callback);
}

Object *JSON::makeObject(const Object::Allocator &allocator) noexcept {
    destructor();
    m_type = Type::OBJECT;

    try {
        new (&m_value.object) Object(allocator);
        return &m_value.object;
    } catch(...) {
        return nullptr;
    }
}

Array *JSON::makeArray(const Array::Allocator &allocator) noexcept {
    destructor();
    m_type = Type::ARRAY;

    try {
        new (&m_value.array) Array(allocator);
        return &m_value.array;
    } catch(...) {
        return nullptr;
    }
}

String *JSON::makeString(const String::Allocator &allocator) noexcept {
    destructor();
    m_type = Type::STRING;

    try {
        new (&m_value.string) String(allocator);
        return &m_value.string;
    } catch(...) {
        return nullptr;
    }
}

JSON &JSON::set(Error error) noexcept {
    destructor();
    m_type  = Type::ERROR;
    m_value = error;

    return *this;
}

void JSON::destructor() noexcept {
    switch(m_type) {
    case Type::STRING:
        m_value.string.~String();
        return;
    case Type::ARRAY:
        m_value.array.~Array();
        return;
    case Type::OBJECT:
        m_value.object.~Object();
        return;
    default:;
    }
}

void JSON::copy(const JSON &json) {
    m_type = json.m_type;

    switch(m_type) {
    case Type::ERROR:
    case Type::FLOAT64:
    case Type::INT64:
    case Type::UINT64:
    case Type::NUL:
    case Type::BOOL:
        std::memcpy(static_cast<void*>(&m_value), &json.m_value, sizeof(m_value));
        break;
    case Type::STRING:
        m_value = json.m_value.string;
        break;
    case Type::ARRAY:
        m_value = json.m_value.array;
        break;
    case Type::OBJECT:
        m_value = json.m_value.object;
        break;
    }    
}

JSON &JSON::set(const std::string &value, const String::Allocator &allocator) {
    return set(value.c_str(), allocator);
}

JSON &JSON::set(const char *const value, const String::Allocator &allocator) {
    if(m_type != Type::STRING) {
        destructor();
        new (&m_value.string) String(allocator);
        m_type = Type::STRING;
    }
    m_value.string = value;

    return *this;
}

JSON &JSON::set(const String &value) {
    return set(value.getData(), value.getAllocator());
}

JSON &JSON::set(JSON &&value) {
    return this->operator=(std::move(value));
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

JSON::Value::Value(const JSON::Error value) noexcept :
error(value)
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

JSON::Value &JSON::Value::operator=(const JSON::Error value) noexcept {
    error = value;
    return *this;
}

}