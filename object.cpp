#include <cstdlib>

#include "object.hpp"
#include "array.hpp"
#include "string.hpp"
#include "json.hpp"
#include "util.hpp"

namespace CPPJSON {

const unsigned Object::MINIMUM_CAPACITY = 8U;

Arena Object::s_keyArena(Arena::MINIMUM_CAPACITY, Arena::INFINITE_NODES, "Object Keys Arena");
Object::KeyAllocator Object::s_keyAllocator(&s_keyArena);

Object::Object(const Allocator &allocator) :
m_data(0, StringHasher(), StringEqual(), allocator)
{}

Object::Object(Allocator &&allocator) noexcept : 
m_data(0, StringHasher(), StringEqual(), std::move(allocator))
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
        m_data.reserve(std::size_t(capacity));
        return true;
    }
    catch (...) {
        return false;
    }
}

Result<JSON&> Object::get(const String &key) noexcept {
    return has(key)
        ? Result<JSON&>::fromRef(m_data.at(key))
        : Result<JSON&>::fromError(true);
}

Result<const JSON&> Object::get(const String &key) const noexcept {
    return has(key)
        ? Result<const JSON&>::fromRef(m_data.at(key))
        : Result<const JSON&>::fromError(true);
}

void Object::remove(const std::string &key) noexcept {
    remove(key.c_str());
}

void Object::remove(const String &key) noexcept {
     m_data.erase(key);
}

void Object::remove(const char *const key) noexcept {
    assert(key != nullptr);

    String stringKey(s_keyAllocator);
    stringKey = key;
    remove(stringKey);
    s_keyAllocator.reset();
}

Result<JSON&> Object::get(const std::string &key) noexcept {
   return get(key.c_str());
}

Result<String&> Object::getString(const std::string &key) noexcept {
    return getString(key.c_str());
}

Result<Object&> Object::getObject(const std::string &key) noexcept {
    return getObject(key.c_str());
}

Result<Array&> Object::getArray(const std::string &key) noexcept {
    return getArray(key.c_str());
}

Result<String&> Object::getString(const String &key) noexcept {
    return getRef<String, &JSON::asString>(key);
}

Result<Object&> Object::getObject(const String &key) noexcept {
    return getRef<Object, &JSON::asObject>(key);
}

Result<Array&> Object::getArray(const String &key) noexcept {
    return getRef<Array, &JSON::asArray>(key);
}

Result<JSON&> Object::get(const char *const key) noexcept {
    assert(key != nullptr);

    String stringKey(s_keyAllocator);
    stringKey = key;
    const Result<JSON&> ret = get(stringKey);
    s_keyAllocator.reset();

    return ret;
}

Result<String&> Object::getString(const char *const key) noexcept {
    assert(key != nullptr);

    return getRef<String, &JSON::asString>(key);
}

Result<Object &> Object::getObject(const char *const key) noexcept {
    assert(key != nullptr);

    return getRef<Object, &JSON::asObject>(key);
}

Result<Array&> Object::getArray(const char *const key) noexcept {
    assert(key != nullptr);

    return getRef<Array, &JSON::asArray>(key);
}

Result<const JSON&> Object::get(const std::string &key) const noexcept {
    return get(key.c_str());
}

Result<const String&> Object::getString(const std::string &key) const noexcept {
    return getString(key.c_str());
}

Result<double> Object::getFloat64(const std::string &key) const noexcept {
    return getFloat64(key.c_str());
}

Result<std::int64_t> Object::getInt64(const std::string &key) const noexcept {
    return getInt64(key.c_str());
}

Result<std::uint64_t> Object::getUint64(const std::string &key) const noexcept {
    return getUint64(key.c_str());
}

Result<const Object&> Object::getObject(const std::string &key) const noexcept {
    return getObject(key.c_str());
}

Result<const Array&> Object::getArray(const std::string &key) const noexcept {
    return getArray(key.c_str());
}

Result<std::nullptr_t> Object::getNull(const std::string &key) const noexcept {
    return getNull(key.c_str());
}

Result<bool> Object::getBool(const std::string &key) const noexcept {
    return getBool(key.c_str());
}

Result<const String&> Object::getString(const String &key) const noexcept {
    return getConstRef<String, &JSON::asString>(key);
}

Result<double> Object::getFloat64(const String &key) const noexcept {
    return getValue<double, &JSON::asFloat64>(key);
}

Result<std::int64_t> Object::getInt64(const String &key) const noexcept {
    return getValue<std::int64_t, &JSON::asInt64>(key);
}

Result<std::uint64_t> Object::getUint64(const String &key) const noexcept {
    return getValue<std::uint64_t, &JSON::asUint64>(key);
}

Result<const Object&> Object::getObject(const String &key) const noexcept {
    return getConstRef<Object, &JSON::asObject>(key);
}

Result<const Array&> Object::getArray(const String &key) const noexcept {
    return getConstRef<Array, &JSON::asArray>(key);
}

Result<std::nullptr_t> Object::getNull(const String &key) const noexcept {
    return getValue<std::nullptr_t, &JSON::asNull>(key);
}

Result<bool> Object::getBool(const String &key) const noexcept {
    return getValue<bool, &JSON::asBool>(key);
}

Result<const JSON&> Object::get(const char *const key) const noexcept {
    assert(key != nullptr);

    String stringKey(s_keyAllocator);
    stringKey = key;
    const Result<const JSON&> ret = get(stringKey);
    s_keyAllocator.reset();

    return ret;
}

Result<const String&> Object::getString(const char *const key) const noexcept {
    return getConstRef<String, &JSON::asString>(key); 
}

Result<double> Object::getFloat64(const char *const key) const noexcept {
    return getValue<double, &JSON::asFloat64>(key);
}

Result<std::int64_t> Object::getInt64(const char *const key) const noexcept {
    return getValue<std::int64_t, &JSON::asInt64>(key);
}

Result<std::uint64_t> Object::getUint64(const char *const key) const noexcept {
    return getValue<std::uint64_t, &JSON::asUint64>(key);
}

Result<const Object&> Object::getObject(const char *const key) const noexcept {
    return getConstRef<Object, &JSON::asObject>(key);
}

Result<const Array&> Object::getArray(const char *const key) const noexcept {
    return getConstRef<Array, &JSON::asArray>(key);
}

Result<std::nullptr_t> Object::getNull(const char *const key) const noexcept {
    return getValue<std::nullptr_t, &JSON::asNull>(key);
}

Result<bool> Object::getBool(const char *const key) const noexcept {
    return getValue<bool, &JSON::asBool>(key);
}

JSON &Object::unsafeGet(const std::string &key) {
    return unsafeGet(key.c_str());
}

String &Object::unsafeGetString(const std::string &key) {
    return unsafeGetString(key.c_str());
}

Object &Object::unsafeGetObject(const std::string &key) {
    return unsafeGetObject(key.c_str());
}

Array &Object::unsafeGetArray(const std::string &key) {
    return unsafeGetArray(key.c_str());
}

JSON &Object::unsafeGet(const String &key) {
    assert(has(key));

    return m_data.at(key);
}

String &Object::unsafeGetString(const String &key) {
    assert(has(key));

    return unsafeGet(key).unsafeAsString();
}

Object &Object::unsafeGetObject(const String &key) {
    assert(has(key));
    
    return unsafeGet(key).unsafeAsObject();
}

Array &Object::unsafeGetArray(const String &key) {
    assert(has(key));
    
    return unsafeGet(key).unsafeAsArray();
}

JSON &Object::unsafeGet(const char *const key) {
    assert(key != nullptr);

    String stringKey(s_keyAllocator);
    stringKey = key;
    JSON &ret = unsafeGet(stringKey);
    s_keyAllocator.reset();

    return ret;
}

String &Object::unsafeGetString(const char *const key) {
    assert(key != nullptr);

    return unsafeGet(key).unsafeAsString();
}

Object &Object::unsafeGetObject(const char *const key) {
    assert(key != nullptr);

    return unsafeGet(key).unsafeAsObject();
}

Array &Object::unsafeGetArray(const char *const key) {
    assert(key != nullptr);

    return unsafeGet(key).unsafeAsArray();
}

const JSON &Object::unsafeGet(const std::string &key) const noexcept {
    return unsafeGet(key.c_str());
}

const String &Object::unsafeGetString(const std::string &key) const noexcept {
    return unsafeGetString(key.c_str());
}

double Object::unsafeGetFloat64(const std::string &key) const noexcept {
    return unsafeGetFloat64(key.c_str());
}

std::int64_t Object::unsafeGetInt64(const std::string &key) const noexcept {
    return unsafeGetInt64(key.c_str());
}

std::uint64_t Object::unsafeGetUint64(const std::string &key) const noexcept {
    return unsafeGetUint64(key.c_str());
}

const Object &Object::unsafeGetObject(const std::string &key) const noexcept {
    return unsafeGetObject(key.c_str());
}

const Array &Object::unsafeGetArray(const std::string &key) const noexcept {
    return unsafeGetArray(key.c_str());
}

std::nullptr_t Object::unsafeGetNull(const std::string &key) const noexcept {
    return unsafeGetNull(key.c_str());
}

bool Object::unsafeGetBool(const std::string &key) const noexcept {
    return unsafeGetBool(key.c_str());
}

const JSON &Object::unsafeGet(const String &key) const noexcept {
    assert(has(key) > 0);

    try {
        return m_data.at(key);
    } catch(...) {
        return JSON::INVALID_JSON;
    }
}

const String & Object::unsafeGetString (const String &key) const noexcept {
    assert(has(key) > 0);

    return unsafeGet(key).unsafeAsString();
}

double Object::unsafeGetFloat64(const String &key) const noexcept {
    assert(has(key) > 0);

    return unsafeGet(key).unsafeAsFloat64();
}

std::int64_t Object::unsafeGetInt64(const String &key) const noexcept {
    assert(has(key) > 0);

    return unsafeGet(key).unsafeAsInt64();
}

std::uint64_t Object::unsafeGetUint64(const String &key) const noexcept {
    assert(has(key) > 0);

    return unsafeGet(key).unsafeAsUint64();
}

const Object &Object::unsafeGetObject(const String &key) const noexcept {
    return unsafeGet(key).unsafeAsObject();
}

const Array &Object::unsafeGetArray(const String &key) const noexcept {
    return unsafeGet(key).unsafeAsArray();
}

std::nullptr_t Object::unsafeGetNull(const String &key) const noexcept {
    return unsafeGet(key).unsafeAsNull();
}

bool Object::unsafeGetBool(const String &key) const noexcept {
    return unsafeGet(key).unsafeAsBool();
}

const JSON &Object::unsafeGet(const char *const key) const noexcept {
    assert(key != nullptr);

    String stringKey(s_keyAllocator);
    stringKey = key;
    const JSON &ret = unsafeGet(stringKey);
    s_keyAllocator.reset();

    return ret;
}

const String &Object::unsafeGetString(const char *const key) const noexcept {
    assert(key != nullptr);

    return unsafeGet(key).unsafeAsString();
}

double Object::unsafeGetFloat64(const char *const key) const noexcept {
    assert(key != nullptr);

    return unsafeGet(key).unsafeAsFloat64();
}

std::int64_t Object::unsafeGetInt64(const char *const key) const noexcept {
    assert(key != nullptr);

    return unsafeGet(key).unsafeAsInt64();
}

std::uint64_t Object::unsafeGetUint64(const char *const key) const noexcept {
    assert(key != nullptr);

    return unsafeGet(key).unsafeAsUint64();
}

const Object &Object::unsafeGetObject(const char *const key) const noexcept {
    assert(key != nullptr);

    return unsafeGet(key).unsafeAsObject();    
}

const Array &Object::unsafeGetArray(const char *const key) const noexcept {
    assert(key != nullptr);

    return unsafeGet(key).unsafeAsArray();    
}

std::nullptr_t Object::unsafeGetNull(const char *const key) const noexcept {
    assert(key != nullptr);

    return unsafeGet(key).unsafeAsNull();
}

bool Object::unsafeGetBool(const char *const key) const noexcept {
    assert(key != nullptr);

    return unsafeGet(key).unsafeAsBool();
}

bool Object::set(const String &key, JSON &&value) noexcept {
    return setRef(key, std::move(value));
}

bool Object::set(const String &key, String &&value) noexcept {
    return setRef(key, std::move(value));
}

bool Object::set(const String &key, const double value) noexcept {
    return setValue(key, value);
}

bool Object::set(const String &key, const std::int64_t value) noexcept {
    return setValue(key, value);
}

bool Object::set(const String &key, const std::uint64_t value) noexcept {
    return setValue(key, value);
}

bool Object::set(const String &key, const int value) noexcept {
    return set(key, std::int64_t(value));
}

bool Object::set(const String &key, const unsigned value) noexcept {
    return set(key, std::uint64_t(value));
}

bool Object::set(const String &key, Object &&value) noexcept {
    return setRef(key, std::move(value));
}

bool Object::set(const String &key, Array &&value) noexcept {
    return setRef(key, std::move(value));
}

bool Object::set(const String &key, const std::nullptr_t) noexcept {
    return setValue(key, nullptr);
}

bool Object::set(const String &key, const bool value) noexcept {
    return setValue(key, value);
}

bool Object::set(String &&key, JSON &&value) noexcept {
    return setRef(std::move(key), std::move(value));
}

bool Object::set(String &&key, String &&value) noexcept {
    return setRef(std::move(key), std::move(value));
}

bool Object::set(String &&key, const double value) noexcept {
    return setValue(std::move(key), value);
}

bool Object::set(String &&key, const std::int64_t value) noexcept {
    return setValue(std::move(key), value);
}

bool Object::set(String &&key, const std::uint64_t value) noexcept {
    return setValue(std::move(key), value);
}

bool Object::set(String &&key, const int value) noexcept {
    return set(std::move(key), std::int64_t(value));
}

bool Object::set(String &&key, const unsigned value) noexcept {
    return set(std::move(key), std::uint64_t(value));
}

bool Object::set(String &&key, Object &&value) noexcept {
    return setRef(std::move(key), std::move(value));
}

bool Object::set(String &&key, Array &&value) noexcept {
    return setRef(std::move(key), std::move(value));
}

bool Object::set(String &&key, const std::nullptr_t) noexcept {
    return setValue(std::move(key), nullptr);
}

bool Object::set(String &&key, const bool value) noexcept {
    return setValue(std::move(key), value);
}

bool Object::has(const std::string &key) const noexcept {
    return has(key.c_str());
}

bool Object::has(const char *const key) const noexcept {
    assert(key != nullptr);

    String stringKey(s_keyAllocator);
    stringKey = key;
    const bool ret = has(stringKey);
    s_keyAllocator.reset();

    return ret;
}

bool Object::has(const String &key) const noexcept {
    return m_data.count(key) > 0;
}

unsigned Object::size() const noexcept {
    return unsigned(m_data.size());
}

const JSON &Object::operator[](const std::string &key) const noexcept {
    return (*this)[key.c_str()];
}

const JSON &Object::operator[](const char *const key) const noexcept {
    assert(key != nullptr);

    String stringKey(s_keyAllocator);
    stringKey = key;
    const JSON &ret = (*this)[stringKey];
    s_keyAllocator.reset();

    return ret;
}

JSON &Object::operator[](const String &key) { 
    return m_data[key];
}

JSON &Object::operator[](String &&key) noexcept { 
    return m_data[std::move(key)];
}

const JSON &Object::operator[](const String &key) const noexcept { 
    try {
        return m_data.at(key);
    } catch(...) {
        return JSON::INVALID_JSON;
    }
}

unsigned Object::toStringSize(const unsigned indentation, const unsigned level) const noexcept {
    const unsigned entryCount = size();
    unsigned stringSize = unsigned(static_strlen("{") + static_strlen("}"));

    if(entryCount > 0U) {
        const unsigned colonSize = indentation > 0U
            ? unsigned(static_strlen(": "))
            : unsigned(static_strlen(":"));

        stringSize += (entryCount - 1U) * unsigned(static_strlen(","));
        stringSize += entryCount        * colonSize;

        if(indentation > 0U) {
            const unsigned whiteSpace = indentation * level;
            stringSize += unsigned(static_strlen("\n") + whiteSpace) * entryCount + unsigned(static_strlen("\n") + whiteSpace - indentation);
        }
    }

    for(const KeyValueType &keyValue : *this) {
        stringSize += keyValue.first.toStringSize();
        stringSize += keyValue.second.toStringSize(indentation, level + 1U);
    }

    return stringSize;
}

void Object::toString(std::string &string, const unsigned indentation,const unsigned level) const noexcept {
    string.push_back('{');

    if(size() == 0U) {
        string.push_back('}');
        return;
    }

    if(indentation > 0U) {
        std::size_t whitespaceSize(indentation * level);
    
        for(const KeyValueType &keyValue : *this) {
            string.push_back('\n');
            string.append(whitespaceSize, ' ');
            keyValue.first.toString(string);
            string.push_back(':');
            string.push_back(' ');
            keyValue.second.toString(string, indentation, level + 1U);
            string.push_back(',');
        }

        string[string.size() - 1] = '\n';
        whitespaceSize = std::size_t(indentation * (level - 1U));
        string.append(whitespaceSize, ' ');
        string.push_back('}');
        return;
    }

    for(const KeyValueType &keyValue : *this) {
        keyValue.first.toString(string);
        string.push_back(':');
        keyValue.second.toString(string);
        string.push_back(',');
    }
    string[string.size() - 1] = '}';
}

Object::Allocator Object::getAllocator() const noexcept {
    return m_data.get_allocator();
}

Object::iterator Object::begin() noexcept {
    return m_data.begin();
}

Object::const_iterator Object::begin() const noexcept {
    return m_data.begin();
}

Object::iterator Object::end() noexcept {
    return m_data.end();
}

Object::const_iterator Object::end() const noexcept {
    return m_data.end();
}

void Object::destructor() noexcept {
    m_data.~Container();
}

Object::KeyAllocator &Object::getKeyAllocator() {
    return s_keyAllocator;
}

}