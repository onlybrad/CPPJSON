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
    return get(key.getData());
}

Object::ValueType *Object::get(const char *const key) noexcept {
    assert(key != nullptr);

    if(m_data.count(key) == 0) {
        return nullptr;
    }

    return &m_data[key];
}

void Object::remove(const std::string &key) noexcept {
    return remove(key.c_str());
}

void Object::remove(const String &key) noexcept {
    return remove(key.getData());
}

void Object::remove(const char *const key) noexcept {
    assert(key != nullptr);

    m_data.erase(key);
}

String *Object::getString(const std::string &key, bool &success) noexcept {
    return getString(key.c_str(), success);
}

double Object::getFloat64(const std::string &key, bool &success) noexcept {
    return getFloat64(key.c_str(), success);
}

std::int64_t Object::getInt64(const std::string &key, bool &success) noexcept {
    return getInt64(key.c_str(), success);
}

std::uint64_t Object::getUint64(const std::string &key, bool &success) noexcept {
    return getUint64(key.c_str(), success);
}

Object *Object::getObject(const std::string &key, bool &success) noexcept {
    return getObject(key.c_str(), success);
}

Array *Object::getArray(const std::string &key, bool &success) noexcept {
    return getArray(key.c_str(), success);
}

std::nullptr_t Object::getNull(const std::string &key, bool &success) noexcept {
    return getNull(key.c_str(), success);
}

bool Object::getBool(const std::string &key, bool &success) noexcept {
    return getBool(key.c_str(), success);
}

String *Object::getString(const String &key, bool &success) noexcept {
    return getString(key.getData(), success);
}

double Object::getFloat64(const String &key, bool &success) noexcept {
    return getFloat64(key.getData(), success);
}

std::int64_t Object::getInt64(const String &key, bool &success) noexcept {
    return getInt64(key.getData(), success);
}

std::uint64_t Object::getUint64(const String &key, bool &success) noexcept {
    return getUint64(key.getData(), success);
}

Object *Object::getObject(const String &key, bool &success) noexcept {
    return getObject(key.getData(), success);
}

Array *Object::getArray(const String &key, bool &success) noexcept {
    return getArray(key.getData(), success);
}

std::nullptr_t Object::getNull(const String &key, bool &success) noexcept {
    return getNull(key.getData(), success);
}

bool Object::getBool(const String &key, bool &success) noexcept {
    return getBool(key.getData(), success);
}

String *Object::getString(const char *const key, bool &success) noexcept {
    assert(key != nullptr);

    JSON *const json = (*this)[key];
    return json->asString(success);
}

double Object::getFloat64(const char *const key, bool &success) noexcept {
    assert(key != nullptr);

    JSON *const json = (*this)[key];
    return json->asFloat64(success);
}

std::int64_t Object::getInt64(const char *const key, bool &success) noexcept {
    assert(key != nullptr);

    JSON *const json = (*this)[key];
    return json->asInt64(success);
}

std::uint64_t Object::getUint64(const char *const key, bool &success) noexcept {
    assert(key != nullptr);

    JSON *const json = (*this)[key];
    return json->asUint64(success);
}

Object *Object::getObject(const char *const key, bool &success) noexcept {
    assert(key != nullptr);

    JSON *const json = (*this)[key];
    return json->asObject(success);
}

Array *Object::getArray(const char *const key, bool &success) noexcept {
    assert(key != nullptr);

    JSON *const json = (*this)[key];
    return json->asArray(success);
}

std::nullptr_t Object::getNull(const char *const key, bool &success) noexcept {
    assert(key != nullptr);

    JSON *const json = (*this)[key];
    return json->asNull(success);
}

bool Object::getBool(const char *const key, bool &success) noexcept {
    assert(key != nullptr);

    JSON *const json = (*this)[key];
    return json->asBool(success);
}

String *Object::getString(const std::string &key) noexcept {
    bool success;
    return getString(key, success);
}

double Object::getFloat64(const std::string &key) noexcept {
    bool success;
    return getFloat64(key, success);
}

std::int64_t Object::getInt64(const std::string &key) noexcept {
    bool success;
    return getInt64(key, success);
}

std::uint64_t Object::getUint64(const std::string &key) noexcept {
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

std::nullptr_t Object::getNull(const std::string &key) noexcept {
    bool success;
    return getNull(key, success);
}

bool Object::getBool(const std::string &key) noexcept {
    bool success;
    return getBool(key, success);
}

String *Object::getString(const String &key) noexcept {
    bool success;
    return getString(key, success);
}

double Object::getFloat64(const String &key) noexcept {
    bool success;
    return getFloat64(key, success);
}

std::int64_t Object::getInt64(const String &key) noexcept {
    bool success;
    return getInt64(key, success);
}

std::uint64_t Object::getUint64(const String &key) noexcept {
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

std::nullptr_t Object::getNull(const String &key) noexcept {
    bool success;
    return getNull(key, success);
}

bool Object::getBool(const String &key) noexcept {
    bool success;
    return getBool(key, success);
}

String *Object::getString(const char *const key) noexcept {
    assert(key != nullptr);

    bool success;
    return getString(key, success);
}

double Object::getFloat64(const char *const key) noexcept {
    assert(key != nullptr);

    bool success;
    return getFloat64(key, success);
}

std::int64_t Object::getInt64(const char *const key) noexcept {
    assert(key != nullptr);

    bool success;
    return getInt64(key, success);
}

std::uint64_t Object::getUint64(const char *const key) noexcept {
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

std::nullptr_t Object::getNull(const char *const key) noexcept {
    assert(key != nullptr);

    bool success;
    return getNull(key, success);
}

bool Object::getBool(const char *const key) noexcept {
    assert(key != nullptr);

    bool success;
    return getBool(key, success);
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
    return set(key.getData(), std::move(value));
}

bool Object::set(const String &key, String &&value) noexcept {
    return set(key.getData(), std::move(value));
}

bool Object::set(const String &key, const double value) noexcept {
    return set(key.getData(), value);
}

bool Object::set(const String &key, const int64_t value) noexcept {
    return set(key.getData(), value);
}

bool Object::set(const String &key, const uint64_t value) noexcept {
    return set(key.getData(), value);
}

bool Object::set(const String &key, const int value) noexcept {
    return set(key.getData(), value);
}

bool Object::set(const String &key, const unsigned value) noexcept {
    return set(key.getData(), value);
}

bool Object::set(const String &key, Object &&value) noexcept {
    return set(key.getData(), std::move(value));
}

bool Object::set(const String &key, Array &&value) noexcept {
    return set(key.getData(), std::move(value));
}

bool Object::set(const String &key, const std::nullptr_t) noexcept {
    return set(key.getData());
}

bool Object::set(const String &key, const bool value) noexcept {
    return set(key.getData(), value);
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
    return this->operator[](key.c_str());
}

Object::ValueType *Object::operator[](const String &key) noexcept { 
    return this->operator[](key.getData());
}

Object::ValueType *Object::operator[](String &&key) noexcept { 
    return this->operator[](std::move(key.getContainer()));
}

Object::ValueType *Object::operator[](String::Container &&key) noexcept {
    try {
        return &m_data.operator[](std::move(key));
    } catch(...) {
        return nullptr;
    }
}

Object::ValueType *Object::operator[](const char *const key) noexcept {
    assert(key != nullptr);

    try {
        return &m_data.operator[](key);
    } catch(...) {
        return nullptr;
    }
}

Object::Allocator Object::getAllocator() const noexcept { 
    return m_data.get_allocator();
}

}