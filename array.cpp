#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "array.hpp"
#include "object.hpp"
#include "string.hpp"
#include "json.hpp"
#include "util.hpp"

namespace CPPJSON {

const unsigned Array::MINIMUM_CAPACITY = 8U;

Array::Array(const Allocator &allocator) :
m_data(0, allocator)
{}

Array::Array(Allocator &&allocator) noexcept :
m_data(0, std::move(allocator))
{}

Array::~Array() noexcept {}

Array &Array::operator=(const Array &array) {
    m_data = array.m_data;

    return *this;
}

Array &Array::operator=(Array &&array) noexcept {
    if(this != &array) {
        m_data = std::move(array.m_data);
    }

    return *this;
}

bool Array::reserve(unsigned capacity) noexcept {
    if(capacity < Array::MINIMUM_CAPACITY) {
        capacity = Array::MINIMUM_CAPACITY;
    }

    try {
        m_data.reserve(std::size_t(capacity));
    } catch (...) {
        return false;
    }

    return true;
}

Result<JSON&> Array::get(const unsigned index) noexcept {
    if(index >= size()) {
        return Result<JSON&>::fromError(true);
    }

    return Result<JSON&>::fromRef(m_data[index]);
}

Result<const JSON&> Array::get(const unsigned index) const noexcept {
    if(index >= size()) {
        return Result<const JSON&>::fromError(true);
    }

    return Result<const JSON&>::fromRef(m_data[index]);
}

Result<String&> Array::getString(const unsigned index) noexcept {
    return getRef<String, JSON::asString>(index);
}

Result<double> Array::getFloat64(const unsigned index) const noexcept {
    return getConst<double, JSON::asFloat64>(index);
}

Result<std::int64_t> Array::getInt64(const unsigned index) const noexcept {
    return getConst<std::int64_t, JSON::asInt64>(index);
}

Result<std::uint64_t> Array::getUint64(const unsigned index) const noexcept {
    return getConst<std::uint64_t, JSON::asUint64>(index);
}

Result<Object&> Array::getObject(const unsigned index) noexcept {
    return getRef<Object, JSON::asObject>(index);
}

Result<Array&> Array::getArray(const unsigned index) noexcept {
    return getRef<Array, JSON::asArray>(index);
}

Result<std::nullptr_t> Array::getNull(const unsigned index) const noexcept {
    return getConst<std::nullptr_t, JSON::asNull>(index);
}

Result<bool> Array::getBool(const unsigned index) const noexcept {
    return getConst<bool, JSON::asBool>(index);
}

Result<const String&> Array::getString(const unsigned index) const noexcept {
    return getConstRef<String, JSON::asString>(index);
}

Result<const Object&> Array::getObject(const unsigned index) const noexcept {
    return getConstRef<Object, JSON::asObject>(index);
}

Result<const Array&> Array::getArray(const unsigned index) const noexcept {
    return getConstRef<Array, JSON::asArray>(index);
}

JSON &Array::unsafeGet(const unsigned index) noexcept {
    assert(std::size_t(index) < m_data.size());
    return m_data[index];
}

String &Array::unsafeGetString(const unsigned index) noexcept {
    return unsafeGet(index).unsafeAsString();
}

double Array::unsafeGetFloat64(const unsigned index) const noexcept {
    return unsafeGet(index).unsafeAsFloat64();
}

std::int64_t Array::unsafeGetInt64(const unsigned index) const noexcept {
    return unsafeGet(index).unsafeAsInt64();
}

std::uint64_t Array::unsafeGetUint64(const unsigned index) const noexcept {
    return unsafeGet(index).unsafeAsUint64();
}

Object &Array::unsafeGetObject(const unsigned index) noexcept {
    return unsafeGet(index).unsafeAsObject();
}

Array &Array::unsafeGetArray(const unsigned index) noexcept {
    return unsafeGet(index).unsafeAsArray();
}

std::nullptr_t Array::unsafeGetNull(const unsigned index) const noexcept {
    return unsafeGet(index).unsafeAsNull();
}

bool Array::unsafeGetBool(const unsigned index) const noexcept {
    return unsafeGet(index).unsafeAsBool();
}

const JSON &Array::unsafeGet(const unsigned index) const noexcept {
    assert(std::size_t(index) < m_data.size());
    return m_data[index];
}

const String &Array::unsafeGetString(const unsigned index) const noexcept {
    return unsafeGet(index).unsafeAsString();
}

const Object &Array::unsafeGetObject(const unsigned index) const noexcept {
    return unsafeGet(index).unsafeAsObject();
}

const Array &Array::unsafeGetArray(const unsigned index) const noexcept {
    return unsafeGet(index).unsafeAsArray();
}

bool Array::set(const unsigned index, String &&value) noexcept {
    return setRef(index, std::move(value));
}

bool Array::set(const unsigned index, const double value) noexcept {
    return setValue(index, value);
}

bool Array::set(const unsigned index, const std::int64_t value) noexcept {
    return setValue(index, value);
}

bool Array::set(const unsigned index, const std::uint64_t value) noexcept {
    return setValue(index, value);
}

bool Array::set(const unsigned index, const int value) noexcept { 
    return set(index, std::int64_t(value));
}

bool Array::set(const unsigned index, const unsigned value) noexcept { 
    return set(index, std::uint64_t(value));
}

bool Array::set(const unsigned index, Object &&value) noexcept {
    return setRef(index, std::move(value));
}

bool Array::set(const unsigned index, Array &&value) noexcept {
    return setRef(index, std::move(value));
}

bool Array::set(const unsigned index, const std::nullptr_t) noexcept {
    return setValue(index, nullptr);
}

bool Array::set(const unsigned index, const bool value) noexcept {
    return setValue(index, value);
}

bool Array::push(String &&value) noexcept {
    return pushRef(std::move(value));
}

bool Array::push(const double value) noexcept {
    return pushValue(value);
}

bool Array::push(const std::int64_t value) noexcept {
    return pushValue(value);
}

bool Array::push(const std::uint64_t value) noexcept {
    return pushValue(value);
}

bool Array::push(const int value) noexcept {
    return push(std::int64_t(value));
}

bool Array::push(const unsigned value) noexcept {
    return push(std::uint64_t(value));
}

bool Array::push(Object &&value) noexcept {
    return pushRef(std::move(value));
}

bool Array::push(Array &&value) noexcept {
    return pushRef(std::move(value));
}

bool Array::push(const std::nullptr_t) noexcept {
    return pushValue(nullptr);
}

bool Array::push(const bool value) noexcept {
    return pushValue(value);
}

Result<JSON&> Array::back() noexcept {
    if(size() == 0) {
        return Result<JSON&>::fromError(true);
    }
    return Result<JSON&>::fromRef(m_data.back());
}

JSON &Array::unsafeBack() noexcept {
    return m_data.back();
}

Result<const JSON&> Array::back() const noexcept {
    if(size() == 0) {
        return Result<const JSON&>::fromError(true);
    }
    return Result<const JSON&>::fromRef(m_data.back());
}

const JSON &Array::unsafeBack() const noexcept {
    return m_data.back();
}

unsigned Array::size() const noexcept {
    return unsigned(m_data.size());
}

JSON &Array::operator[](const unsigned index) {
    return *getEntry(index);
}

const JSON& Array::operator[](const unsigned index) const noexcept {
    const JSON *const json = getEntry(index);

    return json == nullptr
        ? JSON::INVALID_JSON
        : *json;
}

unsigned Array::toStringSize(const unsigned indentation, const unsigned level) const noexcept {
    const unsigned arraySize = size();
    unsigned stringSize = unsigned(static_strlen("[") + static_strlen("]"));

    if(arraySize > 0U) {
        stringSize += (arraySize - 1U) * unsigned(static_strlen(","));

        if(indentation > 0U) {
            const unsigned whitespaceSize = indentation * level;
            stringSize += unsigned(static_strlen("\n") + whitespaceSize) * arraySize + unsigned(static_strlen("\n") + whitespaceSize - indentation);
        }
    }

    for(const JSON &json : *this) {
        stringSize += json.toStringSize(indentation, level + 1U);
    }

    return stringSize;
}

void Array::toString(std::string &string, const unsigned indentation, const unsigned level) const noexcept {
    string.push_back('[');

    if(size() == 0U) {
        string.push_back(']');
        return;
    }

    if(indentation > 0U) {
        std::size_t whitespaceSize(indentation * level);

        for(const JSON &json : *this) {
            string.push_back('\n');
            string.append(whitespaceSize, ' ');
            json.toString(string, indentation, level + 1U);
            string.push_back(',');
        }

        string[string.size() - 1] = '\n';
        whitespaceSize = std::size_t(indentation * (level - 1U));
        string.append(whitespaceSize, ' ');
        string.push_back(']');
        return;
    }

    for(const JSON &json : *this) {
        json.toString(string, indentation, level);
        string.push_back(',');
    }
    string[string.size() - 1] = ']';
}


Array::Allocator Array::getAllocator() const noexcept {
    return m_data.get_allocator();
}

Array::iterator Array::begin() noexcept {
    return m_data.data();
}

Array::const_iterator Array::begin() const noexcept {
    return m_data.data();
}

Array::iterator Array::end() noexcept {
    return m_data.data() + size();
}

Array::const_iterator Array::end() const noexcept {
    return m_data.data() + size();
}

JSON *Array::getEntry(const unsigned index) { 
    if(index >= size()) {
        m_data.resize(index + 1);
    }

    return &m_data[index];
}

const JSON *Array::getEntry(const unsigned index) const noexcept { 
    if(index >= size()) {
        return nullptr;
    }

    return &m_data[index];
}

void Array::destructor() noexcept {
    m_data.~Container();
}

template<typename T>
bool Array::setValue(const unsigned index, T value) noexcept {
    JSON *const json = getEntry(index);
    if(json == nullptr) {
        return false;
    }

    json->set(value);
    return true;
}

template<typename T>
bool Array::setRef(const unsigned index, T &&value, typename std::enable_if<!std::is_lvalue_reference<T>::value>::type*) noexcept {
    JSON *const json = getEntry(index);
    if(json == nullptr) {
        return false;
    }

    json->set(std::move(value));
    return true;
}

template<typename T>
bool Array::pushValue(T value) noexcept {
    try {
        m_data.emplace_back();
        JSON &json = m_data.back();
        json.set(value);
        return true;
    } catch(...) {
        return false;
    }
}

template<typename T>
bool Array::pushRef(T &&value, typename std::enable_if<!std::is_lvalue_reference<T>::value>::type*) noexcept {
    try {
        m_data.emplace_back();
        JSON &json = m_data.back();
        json.set(std::move(value));
        return true;
    } catch(...) {
        return false;
    }
}

}