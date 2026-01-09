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

constexpr unsigned Array::MINIMUM_CAPACITY = 8U;

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
        m_data.reserve(static_cast<std::size_t>(capacity));
    } catch (...) {
        return false;
    }

    return true;
}

JSON *Array::get(const unsigned index) noexcept {
    if(index >= size()) {
        return nullptr;
    }

    return &m_data[index];
}

const JSON *Array::get(const unsigned index) const noexcept {
    return const_cast<const JSON*>(const_cast<Array*>(this)->get(index));
}

String *Array::getString(const unsigned index, bool &success) noexcept {
    JSON *const json = get(index);
    return json->asString(success);
}

double Array::getFloat64(const unsigned index, bool &success) const noexcept {
    const JSON *const json = get(index);
    return json->asFloat64(success);
}

std::int64_t Array::getInt64(const unsigned index, bool &success) const noexcept {
    const JSON *const json = get(index);
    return json->asInt64(success);
}

std::uint64_t Array::getUint64(const unsigned index, bool &success) const noexcept {
    const JSON *const json = get(index);
    return json->asUint64(success);
}

Object *Array::getObject(const unsigned index, bool &success) noexcept {
    JSON *const json = get(index);
    return json->asObject(success);
}

Array *Array::getArray(const unsigned index, bool &success) noexcept {
    JSON *const json = get(index);
    return json->asArray(success);
}

std::nullptr_t Array::getNull(const unsigned index, bool &success) const noexcept {
    const JSON *const json = get(index);
    return json->asNull(success);
}

bool Array::getBool(const unsigned index, bool &success) const noexcept {
    const JSON *const json = get(index);
    return json->asBool(success);
}

String *Array::getString(unsigned index) noexcept {
    bool success;
    return getString(index, success);
}

double Array::getFloat64(unsigned index) const noexcept {
    bool success;
    return getFloat64(index, success);
}

std::int64_t Array::getInt64(unsigned index) const noexcept {
    bool success;
    return getInt64(index, success);
}

std::uint64_t Array::getUint64(unsigned index) const noexcept {
    bool success;
    return getUint64(index, success);
}

Object *Array::getObject(unsigned index) noexcept {
    bool success;
    return getObject(index, success);
}

Array *Array::getArray(unsigned index) noexcept {
    bool success;
    return getArray(index, success);
}

std::nullptr_t Array::getNull(unsigned index) const noexcept {
    bool success;
    return getNull(index, success);
}

bool Array::getBool(unsigned index) const noexcept {
    bool success;
    return getBool(index, success);
}

bool Array::getString(unsigned index, GetCallbackRef<String> callback) noexcept {
    assert(callback != nullptr);
    
    return get<String, &JSON::asString>(index, callback);
}

bool Array::getFloat64(unsigned index, GetCallback<double> callback) const noexcept {
    assert(callback != nullptr);

    return get<double, &JSON::asFloat64>(index, callback);
}

bool Array::getInt64(unsigned index, GetCallback<std::int64_t> callback) const noexcept {
    assert(callback != nullptr);

    return get<std::int64_t, &JSON::asInt64>(index, callback);
}

bool Array::getUint64(unsigned index, GetCallback<std::uint64_t> callback) const noexcept {
    assert(callback != nullptr);

    return get<std::uint64_t, &JSON::asUint64>(index, callback);
}

bool Array::getObject(unsigned index, GetCallbackRef<Object> callback) noexcept {
    assert(callback != nullptr);

    return get<Object, &JSON::asObject>(index, callback);
}

bool Array::getArray(unsigned index, GetCallbackRef<Array> callback) noexcept {
    assert(callback != nullptr);

    return get<Array, &JSON::asArray>(index, callback);
}

bool Array::getNull(unsigned index, GetCallback<std::nullptr_t> callback) const noexcept {
    assert(callback != nullptr);

    return get<std::nullptr_t, &JSON::asNull>(index, callback);
}

bool Array::getBool(unsigned index, GetCallback<bool> callback) const noexcept {
    assert(callback != nullptr);

    return get<bool, &JSON::asBool>(index, callback);
}

bool Array::getString(unsigned index, GetCallbackRef<String> callback, FailureCallback failureCallback) noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return get<String, &JSON::asString>(index, callback, failureCallback);
}

bool Array::getFloat64(unsigned index, GetCallback<double> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return get<double, &JSON::asFloat64>(index, callback, failureCallback);
}

bool Array::getInt64(unsigned index, GetCallback<std::int64_t> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return get<std::int64_t, &JSON::asInt64>(index, callback, failureCallback);
}

bool Array::getUint64(unsigned index, GetCallback<std::uint64_t> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);
    
    return get<std::uint64_t, &JSON::asUint64>(index, callback, failureCallback);
}

bool Array::getObject(unsigned index, GetCallbackRef<Object> callback, FailureCallback failureCallback)noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return get<Object, &JSON::asObject>(index, callback, failureCallback);
}

bool Array::getArray(unsigned index, GetCallbackRef<Array> callback, FailureCallback failureCallback) noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return get<Array, &JSON::asArray>(index, callback, failureCallback);
}

bool Array::getNull(unsigned index, GetCallback<std::nullptr_t> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return get<std::nullptr_t, &JSON::asNull>(index, callback, failureCallback);
}

bool Array::getBool(unsigned index, GetCallback<bool> callback, FailureCallback failureCallback) const noexcept {
    assert(callback != nullptr);
    assert(failureCallback != nullptr);

    return get<bool, &JSON::asBool>(index, callback, failureCallback);
}

bool Array::set(const unsigned index, String &&value) noexcept {
    JSON *const json = getEntry(index);
    if(json == nullptr) {
        return false;
    }

    json->set(std::move(value));
    return true;
}

bool Array::set(const unsigned index, const double value) noexcept {
    JSON *const json = getEntry(index);
    if(json == nullptr) {
        return false;
    }

    json->set(value);
    return true;
}

bool Array::set(const unsigned index, const int64_t value) noexcept {
    JSON *const json = getEntry(index);
    if(json == nullptr) {
        return false;
    }

    json->set(value);
    return true;
}

bool Array::set(const unsigned index, const uint64_t value) noexcept {
    JSON *const json = getEntry(index);
    if(json == nullptr) {
        return false;
    }

    json->set(value);
    return true;
}

bool Array::set(const unsigned index, const int value) noexcept { 
    return set(index, static_cast<std::int64_t>(value));
}

bool Array::set(const unsigned index, const unsigned value) noexcept { 
    return set(index, static_cast<uint64_t>(value));
}

bool Array::set(const unsigned index, Object &&value) noexcept {
    JSON *const json = getEntry(index);
    if(json == nullptr) {
        return false;
    }

    json->set(std::move(value));
    return true;
}

bool Array::set(const unsigned index, Array &&value) noexcept {
    JSON *const json = getEntry(index);
    if(json == nullptr) {
        return false;
    }

    json->set(std::move(value));
    return true;
}

bool Array::set(const unsigned index, std::nullptr_t) noexcept {
    JSON *const json = getEntry(index);
    if(json == nullptr) {
        return false;
    }

    json->set();
    return true;
}

bool Array::set(const unsigned index, const bool value) noexcept {
    JSON *const json = getEntry(index);
    if(json == nullptr) {
        return false;
    }

    json->set(value);
    return true;
}

bool Array::push(String &&value) noexcept {
    try {
        m_data.emplace_back();
        JSON &json = m_data.back();
        json.set(std::move(value));
        return true;
    } catch(...) {
        return false;
    }
}

bool Array::push(const double value) noexcept {
    try {
        m_data.emplace_back();
        JSON &json = m_data.back();
        json.set(value);
        return true;
    } catch(...) {
        return false;
    }
}

bool Array::push(const int64_t value) noexcept {
    try {
        m_data.emplace_back();
        JSON &json = m_data.back();
        json.set(value);
        return true;
    } catch(...) {
        return false;
    }
}

bool Array::push(const uint64_t value) noexcept {
    try {
        m_data.emplace_back();
        JSON &json = m_data.back();
        json.set(value);
        return true;
    } catch(...) {
        return false;
    }
}

bool Array::push(const int value) noexcept {
    return push(static_cast<std::int64_t>(value));
}

bool Array::push(const unsigned value) noexcept {
    return push(static_cast<std::uint64_t>(value));
}

bool Array::push(Object &&value) noexcept {
    try {
        m_data.emplace_back();
        JSON &json = m_data.back();
        json.set(std::move(value));
        return true;
    } catch(...) {
        return false;
    }
}

bool Array::push(Array &&value) noexcept {
    try {
        m_data.emplace_back();
        JSON &json = m_data.back();
        json.set(std::move(value));
        return true;
    } catch(...) {
        return false;
    }
}

bool Array::push(const std::nullptr_t) noexcept {
    try {
        m_data.emplace_back();
        JSON &json = m_data.back();
        json.set();
        return true;
    } catch(...) {
        return false;
    }
}

bool Array::push(const bool value) noexcept {
    try {
        m_data.emplace_back();
        JSON &json = m_data.back();
        json.set(value);
        return true;
    } catch(...) {
        return false;
    }
}

Array::ValueType *Array::back() noexcept {
    if(size() == 0) {
        return nullptr;
    }
    return &m_data.back();
}

Array::ValueType &Array::unsafeBack() noexcept {
    return m_data.back();
}

unsigned Array::size() const noexcept {
    return static_cast<unsigned>(m_data.size());
}

Array::ValueType *Array::operator[](const unsigned index) noexcept {
    return getEntry(index);
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

JSON *Array::getEntry(const unsigned index) noexcept { 
    if(index >= size()) {
        try {
            m_data.resize(index + 1);
        } catch(...) {
            return nullptr;
        }
    }

    return &m_data[index];
}

}