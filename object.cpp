#include "object.hpp"
#include "array.hpp"
#include "string.hpp"
#include "json.hpp"
#include "util.hpp"

namespace CPPJSON {

constexpr unsigned Object::MINIMUM_CAPACITY = 8U;

Object::Object(const Allocator &allocator) :
m_data(0, std::hash<KeyType>(), std::equal_to<KeyType>(), allocator)
{}

Object::Object(Allocator &&allocator) noexcept : 
m_data(0, std::hash<KeyType>(), std::equal_to<KeyType>(), std::move(allocator))
{}

Object::~Object() noexcept {}

Object &Object::operator=(const Object &object) {
    m_data = object.m_data;

    return *this;
}

Object &Object::operator=(Object &&object) noexcept {
    if(this != &object) {
        m_data = std::move(object.m_data);
    }

    return *this;
}

bool Object::reserve(unsigned capacity) noexcept {
    if (capacity < Object::MINIMUM_CAPACITY) {
        capacity = Object::MINIMUM_CAPACITY;
    }

    try {
        m_data.reserve(static_cast<std::size_t>(capacity));
        return true;
    }
    catch (...) {
        return false;
    }
}

Object::ValueType *Object::get(const std::string &key) noexcept {
    return get(key.c_str());
}

Object::ValueType *Object::get(const String &key) noexcept {
    return get(key.getCStr());
}

Object::ValueType *Object::get(const char *const key) noexcept {
    assert(key != nullptr);

    if(m_data.count(key) == 0) {
        return nullptr;
    }

    return &m_data[key];
}

const Object::ValueType *Object::get(const char *const key) const noexcept {
    assert(key != nullptr);

    return const_cast<const ValueType*>(const_cast<Object*>(this)->get(key));
}

void Object::remove(const std::string &key) noexcept {
    return remove(key.c_str());
}

void Object::remove(const String &key) noexcept {
    return remove(key.getCStr());
}

void Object::remove(const char *const key) noexcept {
    assert(key != nullptr);

    m_data.erase(key);
}

String *Object::getString(const std::string &key, bool &success) noexcept {
    return getString(key.c_str(), success);
}

double Object::getFloat64(const std::string &key, bool &success) const noexcept {
    return getFloat64(key.c_str(), success);
}

std::int64_t Object::getInt64(const std::string &key, bool &success) const noexcept {
    return getInt64(key.c_str(), success);
}

std::uint64_t Object::getUint64(const std::string &key, bool &success) const noexcept {
    return getUint64(key.c_str(), success);
}

Object *Object::getObject(const std::string &key, bool &success) noexcept {
    return getObject(key.c_str(), success);
}

Array *Object::getArray(const std::string &key, bool &success) noexcept {
    return getArray(key.c_str(), success);
}

std::nullptr_t Object::getNull(const std::string &key, bool &success) const noexcept {
    return getNull(key.c_str(), success);
}

bool Object::getBool(const std::string &key, bool &success) const noexcept {
    return getBool(key.c_str(), success);
}

String *Object::getString(const String &key, bool &success) noexcept {
    return getString(key.getCStr(), success);
}

double Object::getFloat64(const String &key, bool &success) const noexcept {
    return getFloat64(key.getCStr(), success);
}

std::int64_t Object::getInt64(const String &key, bool &success) const noexcept {
    return getInt64(key.getCStr(), success);
}

std::uint64_t Object::getUint64(const String &key, bool &success) const noexcept {
    return getUint64(key.getCStr(), success);
}

Object *Object::getObject(const String &key, bool &success) noexcept {
    return getObject(key.getCStr(), success);
}

Array *Object::getArray(const String &key, bool &success) noexcept {
    return getArray(key.getCStr(), success);
}

std::nullptr_t Object::getNull(const String &key, bool &success) const noexcept {
    return getNull(key.getCStr(), success);
}

bool Object::getBool(const String &key, bool &success) const noexcept {
    return getBool(key.getCStr(), success);
}

String *Object::getString(const char *const key, bool &success) noexcept {
    assert(key != nullptr);

    return get<String, &JSON::asString>(key, success);
}

double Object::getFloat64(const char *const key, bool &success) const noexcept {
    assert(key != nullptr);

    return get<double, &JSON::asFloat64>(key, success);
}

std::int64_t Object::getInt64(const char *const key, bool &success) const noexcept {
    assert(key != nullptr);

    return get<std::int64_t, &JSON::asInt64>(key, success);
}

std::uint64_t Object::getUint64(const char *const key, bool &success) const noexcept {
    assert(key != nullptr);

    return get<std::uint64_t, &JSON::asUint64>(key, success);
}

Object *Object::getObject(const char *const key, bool &success) noexcept {
    assert(key != nullptr);

    return get<Object, &JSON::asObject>(key, success);
}

Array *Object::getArray(const char *const key, bool &success) noexcept {
    assert(key != nullptr);

    return get<Array, &JSON::asArray>(key, success);
}

std::nullptr_t Object::getNull(const char *const key, bool &success) const noexcept {
    assert(key != nullptr);

    return get<std::nullptr_t, &JSON::asNull>(key, success);
}

bool Object::getBool(const char *const key, bool &success) const noexcept {
    assert(key != nullptr);

    return get<bool, &JSON::asBool>(key, success);
}

String *Object::getString(const std::string &key) noexcept {
    bool success;
    return getString(key, success);
}

double Object::getFloat64(const std::string &key) const noexcept {
    bool success;
    return getFloat64(key, success);
}

std::int64_t Object::getInt64(const std::string &key) const noexcept {
    bool success;
    return getInt64(key, success);
}

std::uint64_t Object::getUint64(const std::string &key) const noexcept {
    bool success;
    return getUint64(key, success);
}

Object *Object::getObject(const std::string &key) noexcept {
    bool success;
    return getObject(key, success);
}

Array *Object::getArray(const std::string &key) noexcept {
    bool success;
    return getArray(key, success);
}

std::nullptr_t Object::getNull(const std::string &key) const noexcept {
    bool success;
    return getNull(key, success);
}

bool Object::getBool(const std::string &key) const noexcept {
    bool success;
    return getBool(key, success);
}

String *Object::getString(const String &key) noexcept {
    bool success;
    return getString(key, success);
}

double Object::getFloat64(const String &key) const noexcept {
    bool success;
    return getFloat64(key, success);
}

std::int64_t Object::getInt64(const String &key) const noexcept {
    bool success;
    return getInt64(key, success);
}

std::uint64_t Object::getUint64(const String &key) const noexcept {
    bool success;
    return getUint64(key, success);
}

Object *Object::getObject(const String &key) noexcept {
    bool success;
    return getObject(key, success);
}

Array *Object::getArray(const String &key) noexcept {
    bool success;
    return getArray(key, success);
}

std::nullptr_t Object::getNull(const String &key) const noexcept {
    bool success;
    return getNull(key, success);
}

bool Object::getBool(const String &key) const noexcept {
    bool success;
    return getBool(key, success);
}

String *Object::getString(const char *const key) noexcept {
    assert(key != nullptr);

    bool success;
    return getString(key, success);
}

double Object::getFloat64(const char *const key) const noexcept {
    assert(key != nullptr);

    bool success;
    return getFloat64(key, success);
}

std::int64_t Object::getInt64(const char *const key) const noexcept {
    assert(key != nullptr);

    bool success;
    return getInt64(key, success);
}

std::uint64_t Object::getUint64(const char *const key) const noexcept {
    assert(key != nullptr);

    bool success;
    return getUint64(key, success);
}

Object *Object::getObject(const char *const key) noexcept {
    assert(key != nullptr);

    bool success;
    return getObject(key, success);
}

Array *Object::getArray(const char *const key) noexcept {
    assert(key != nullptr);

    bool success;
    return getArray(key, success);
}

std::nullptr_t Object::getNull(const char *const key) const noexcept {
    assert(key != nullptr);

    bool success;
    return getNull(key, success);
}

bool Object::getBool(const char *const key) const noexcept {
    assert(key != nullptr);

    bool success;
    return getBool(key, success);
}

bool Object::getString(const std::string &key, GetCallbackRef<String> callback) noexcept {
    return get<String, &JSON::asString>(key.c_str(), callback);  
}

bool Object::getFloat64(const std::string &key, GetCallback<double> callback) const noexcept {
    return get<double, &JSON::asFloat64>(key.c_str(), callback);  
}

bool Object::getInt64(const std::string &key, GetCallback<std::int64_t> callback) const noexcept {
    return get<std::int64_t, &JSON::asInt64>(key.c_str(), callback);  
}

bool Object::getUint64(const std::string &key, GetCallback<std::uint64_t> callback) const noexcept {
    return get<std::uint64_t, JSON::asUint64>(key.c_str(), callback);  
}

bool Object::getObject(const std::string &key, GetCallbackRef<Object> callback) noexcept {
    return get<Object, &JSON::asObject>(key.c_str(), callback);  
}

bool Object::getArray(const std::string &key, GetCallbackRef<Array> callback) noexcept {
    return get<Array, &JSON::asArray>(key.c_str(), callback);  
}

bool Object::getNull(const std::string &key, GetCallback<std::nullptr_t> callback) const noexcept {
    return get<std::nullptr_t, &JSON::asNull>(key.c_str(), callback); 
}

bool  Object::getBool(const std::string &key, GetCallback<bool> callback) const noexcept {
    return get<bool, &JSON::asBool>(key.c_str(), callback); 
}

bool Object::getString(const std::string &key, GetCallbackRef<String> callback, FailureCallback failureCallback) noexcept {
    return get<String, &JSON::asString>(key.c_str(), callback, failureCallback);  
}

bool Object::getFloat64(const std::string &key, GetCallback<double> callback, FailureCallback failureCallback) const noexcept {
    return get<double, &JSON::asFloat64>(key.c_str(), callback, failureCallback);  
}

bool Object::getInt64(const std::string &key, GetCallback<std::int64_t> callback, FailureCallback failureCallback) const noexcept {
    return get<std::int64_t, &JSON::asInt64>(key.c_str(), callback, failureCallback);  
}

bool Object::getUint64(const std::string &key, GetCallback<std::uint64_t> callback, FailureCallback failureCallback) const noexcept {
    return get<std::uint64_t, JSON::asUint64>(key.c_str(), callback, failureCallback);  
}

bool Object::getObject(const std::string &key, GetCallbackRef<Object> callback, FailureCallback failureCallback) noexcept {
    return get<Object, &JSON::asObject>(key.c_str(), callback, failureCallback);  
}

bool Object::getArray(const std::string &key, GetCallbackRef<Array> callback, FailureCallback failureCallback) noexcept {
    return get<Array, &JSON::asArray>(key.c_str(), callback, failureCallback);  
}

bool Object::getNull(const std::string &key, GetCallback<std::nullptr_t> callback, FailureCallback failureCallback) const noexcept {
    return get<std::nullptr_t, &JSON::asNull>(key.c_str(), callback, failureCallback); 
}

bool  Object::getBool(const std::string &key, GetCallback<bool> callback, FailureCallback failureCallback) const noexcept {
    return get<bool, &JSON::asBool>(key.c_str(), callback, failureCallback); 
}

bool Object::getString(const String &key, GetCallbackRef<String> callback) noexcept {
    return get<String, &JSON::asString>(key.getCStr(), callback);  
}

bool Object::getFloat64(const String &key, GetCallback<double> callback) const noexcept {
    return get<double, &JSON::asFloat64>(key.getCStr(), callback);  
}

bool Object::getInt64(const String &key, GetCallback<std::int64_t> callback) const noexcept {
    return get<std::int64_t, &JSON::asInt64>(key.getCStr(), callback);  
}

bool Object::getUint64(const String &key, GetCallback<std::uint64_t> callback) const noexcept {
    return get<std::uint64_t, JSON::asUint64>(key.getCStr(), callback);  
}

bool Object::getObject(const String &key, GetCallbackRef<Object> callback) noexcept {
    return get<Object, &JSON::asObject>(key.getCStr(), callback);  
}

bool Object::getArray(const String &key, GetCallbackRef<Array> callback) noexcept {
    return get<Array, &JSON::asArray>(key.getCStr(), callback);  
}

bool Object::getNull(const String &key, GetCallback<std::nullptr_t> callback) const noexcept {
    return get<std::nullptr_t, &JSON::asNull>(key.getCStr(), callback); 
}

bool  Object::getBool(const String &key, GetCallback<bool> callback) const noexcept {
    return get<bool, &JSON::asBool>(key.getCStr(), callback); 
}

bool Object::getString(const String &key, GetCallbackRef<String> callback, FailureCallback failureCallback) noexcept {
    return get<String, &JSON::asString>(key.getCStr(), callback, failureCallback);  
}

bool Object::getFloat64(const String &key, GetCallback<double> callback, FailureCallback failureCallback) const noexcept {
    return get<double, &JSON::asFloat64>(key.getCStr(), callback, failureCallback);  
}

bool Object::getInt64(const String &key, GetCallback<std::int64_t> callback, FailureCallback failureCallback) const noexcept {
    return get<std::int64_t, &JSON::asInt64>(key.getCStr(), callback, failureCallback);  
}

bool Object::getUint64(const String &key, GetCallback<std::uint64_t> callback, FailureCallback failureCallback) const noexcept {
    return get<std::uint64_t, JSON::asUint64>(key.getCStr(), callback, failureCallback);  
}

bool Object::getObject(const String &key, GetCallbackRef<Object> callback, FailureCallback failureCallback) noexcept {
    return get<Object, &JSON::asObject>(key.getCStr(), callback, failureCallback);  
}

bool Object::getArray(const String &key, GetCallbackRef<Array> callback, FailureCallback failureCallback) noexcept {
    return get<Array, &JSON::asArray>(key.getCStr(), callback, failureCallback);  
}

bool Object::getNull(const String &key, GetCallback<std::nullptr_t> callback, FailureCallback failureCallback) const noexcept {
    return get<std::nullptr_t, &JSON::asNull>(key.getCStr(), callback, failureCallback); 
}

bool  Object::getBool(const String &key, GetCallback<bool> callback, FailureCallback failureCallback) const noexcept {
    return get<bool, &JSON::asBool>(key.getCStr(), callback, failureCallback); 
}

bool Object::getString(const char *const key, GetCallbackRef<String> callback) noexcept {
    assert(key != nullptr);

    return get<String, &JSON::asString>(key, callback);  
}

bool Object::getFloat64(const char *const key, GetCallback<double> callback) const noexcept {
    assert(key != nullptr);

    return get<double, &JSON::asFloat64>(key, callback);  
}

bool Object::getInt64(const char *const key, GetCallback<std::int64_t> callback) const noexcept {
    assert(key != nullptr);

    return get<std::int64_t, &JSON::asInt64>(key, callback);  
}

bool Object::getUint64(const char *const key, GetCallback<std::uint64_t> callback) const noexcept {
    assert(key != nullptr);

    return get<std::uint64_t, JSON::asUint64>(key, callback);  
}

bool Object::getObject(const char *const key, GetCallbackRef<Object> callback) noexcept {
    assert(key != nullptr);

    return get<Object, &JSON::asObject>(key, callback);  
}

bool Object::getArray(const char *const key, GetCallbackRef<Array> callback) noexcept {
    assert(key != nullptr);

    return get<Array, &JSON::asArray>(key, callback);  
}

bool Object::getNull(const char *const key, GetCallback<std::nullptr_t> callback) const noexcept {
    assert(key != nullptr);

    return get<std::nullptr_t, &JSON::asNull>(key, callback); 
}

bool  Object::getBool(const char *const key, GetCallback<bool> callback) const noexcept {
    assert(key != nullptr);

    return get<bool, &JSON::asBool>(key, callback); 
}

bool Object::getString(const char *const key, GetCallbackRef<String> callback, FailureCallback failureCallback) noexcept {
    assert(key != nullptr);
    
    return get<String, &JSON::asString>(key, callback, failureCallback);  
}

bool Object::getFloat64(const char *const key, GetCallback<double> callback, FailureCallback failureCallback) const noexcept {
    assert(key != nullptr);
    
    return get<double, &JSON::asFloat64>(key, callback, failureCallback);  
}

bool Object::getInt64(const char *const key, GetCallback<std::int64_t> callback, FailureCallback failureCallback) const noexcept {
    assert(key != nullptr);
    
    return get<std::int64_t, &JSON::asInt64>(key, callback, failureCallback);  
}

bool Object::getUint64(const char *const key, GetCallback<std::uint64_t> callback, FailureCallback failureCallback) const noexcept {
    assert(key != nullptr);
    
    return get<std::uint64_t, JSON::asUint64>(key, callback, failureCallback);  
}

bool Object::getObject(const char *const key, GetCallbackRef<Object> callback, FailureCallback failureCallback) noexcept {
    assert(key != nullptr);
    
    return get<Object, &JSON::asObject>(key, callback, failureCallback);  
}

bool Object::getArray(const char *const key, GetCallbackRef<Array> callback, FailureCallback failureCallback) noexcept {
    assert(key != nullptr);
    
    return get<Array, &JSON::asArray>(key, callback, failureCallback);  
}

bool Object::getNull(const char *const key, GetCallback<std::nullptr_t> callback, FailureCallback failureCallback) const noexcept {
    assert(key != nullptr);
    
    return get<std::nullptr_t, &JSON::asNull>(key, callback, failureCallback); 
}

bool  Object::getBool(const char *const key, GetCallback<bool> callback, FailureCallback failureCallback) const noexcept {
    assert(key != nullptr);
    
    return get<bool, &JSON::asBool>(key, callback, failureCallback); 
}

bool Object::set(const std::string &key, JSON &&value) noexcept {
    return set(key.c_str(), std::move(value));
}

bool Object::set(const std::string &key, String &&value) noexcept {
    return set(key.c_str(), std::move(value));
}

bool Object::set(const std::string &key, const double value) noexcept {
    return set(key.c_str(), value);
}

bool Object::set(const std::string &key, const int64_t value) noexcept {
    return set(key.c_str(), value);
}

bool Object::set(const std::string &key, const uint64_t value) noexcept {
    return set(key.c_str(), value);
}

bool Object::set(const std::string &key, const int value) noexcept {
    return set(key.c_str(), value);
}

bool Object::set(const std::string &key, const unsigned value) noexcept {
    return set(key.c_str(), value);
}

bool Object::set(const std::string &key, Object &&value) noexcept {
    return set(key.c_str(), std::move(value));
}

bool Object::set(const std::string &key, Array &&value) noexcept {
    return set(key.c_str(), std::move(value));
}

bool Object::set(const std::string &key, const std::nullptr_t) noexcept {
    return set(key.c_str());
}

bool Object::set(const std::string &key, const bool value) noexcept {
    return set(key.c_str(), value);
}

bool Object::set(const String &key, JSON &&value) noexcept {
    return set(key.getCStr(), std::move(value));
}

bool Object::set(const String &key, String &&value) noexcept {
    return set(key.getCStr(), std::move(value));
}

bool Object::set(const String &key, const double value) noexcept {
    return set(key.getCStr(), value);
}

bool Object::set(const String &key, const int64_t value) noexcept {
    return set(key.getCStr(), value);
}

bool Object::set(const String &key, const uint64_t value) noexcept {
    return set(key.getCStr(), value);
}

bool Object::set(const String &key, const int value) noexcept {
    return set(key.getCStr(), value);
}

bool Object::set(const String &key, const unsigned value) noexcept {
    return set(key.getCStr(), value);
}

bool Object::set(const String &key, Object &&value) noexcept {
    return set(key.getCStr(), std::move(value));
}

bool Object::set(const String &key, Array &&value) noexcept {
    return set(key.getCStr(), std::move(value));
}

bool Object::set(const String &key, const std::nullptr_t) noexcept {
    return set(key.getCStr());
}

bool Object::set(const String &key, const bool value) noexcept {
    return set(key.getCStr(), value);
}

bool Object::set(const char *const key, JSON &&value) noexcept {
    assert(key != nullptr);

    JSON *const json = (*this)[key];
    if(json == nullptr) {
        return false;
    }

    *json = std::move(value);
    return true;
}

bool Object::set(const char *const key, String &&value) noexcept {
    assert(key != nullptr);
    
    JSON *const json = (*this)[key];
    if(json == nullptr) {
        return false;
    }

    json->set(std::move(value));
    return true;
}

bool Object::set(const char *const key, const double value) noexcept {
    assert(key != nullptr);
    
    JSON *const json = (*this)[key];
    if(json == nullptr) {
        return false;
    }

    json->set(value);
    return true;
}



bool Object::set(const char *const key, const int64_t value) noexcept {
    assert(key != nullptr);
    
    JSON *const json = (*this)[key];
    if(json == nullptr) {
        return false;
    }

    json->set(value);
    return true;
}

bool Object::set(const char *const key, const uint64_t value) noexcept {
    assert(key != nullptr);
    
    JSON *const json = (*this)[key];
    if(json == nullptr) {
        return false;
    }

    json->set(value);
    return true;
}

bool Object::set(const char *const key, const int value) noexcept {
    return set(key, static_cast<std::int64_t>(value));
}

bool Object::set(const char *const key, const unsigned value) noexcept {
    return set(key, static_cast<uint64_t>(value));
}

bool Object::set(const char *const key, Object &&value) noexcept {
    assert(key != nullptr);
    
    JSON *const json = (*this)[key];
    if(json == nullptr) {
        return false;
    }

    json->set(std::move(value));
    return true;
}

bool Object::set(const char *const key, Array &&value) noexcept {
    assert(key != nullptr);
    
    JSON *const json = (*this)[key];
    if(json == nullptr) {
        return false;
    }

    json->set(std::move(value));
    return true;
}

bool Object::set(const char *const key, std::nullptr_t) noexcept {
    assert(key != nullptr);
    
    JSON *const json = (*this)[key];
    if(json == nullptr) {
        return false;
    }

    json->set();
    return true;
}

bool Object::set(const char *const key, const bool value) noexcept {
    assert(key != nullptr);
    
    JSON *const json = (*this)[key];
    if(json == nullptr) {
        return false;
    }

    json->set(value);
    return true;
}

Object::ValueType *Object::operator[](const std::string &key) noexcept {
    return (*this)[key.c_str()];
}

Object::ValueType *Object::operator[](const String &key) noexcept { 
    return (*this)[key.getCStr()];
}

Object::ValueType *Object::operator[](String &&key) noexcept { 
    return (*this)[std::move(key.getContainer())];
}

Object::ValueType *Object::operator[](String::Container &&key) noexcept {
    try {
        return &m_data[std::move(key)];
    } catch(...) {
        return nullptr;
    }
}

Object::ValueType *Object::operator[](const char *const key) noexcept {
    assert(key != nullptr);

    try {
        return &m_data[key];
    } catch(...) {
        return nullptr;
    }
}

Object::Allocator Object::getAllocator() const noexcept { 
    return m_data.get_allocator();
}

}