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
    if(index >= m_data.size()) {
        try {
            m_data.resize(index + 1);
        } catch(...) {
            return nullptr;
        }
    }

    return &m_data[index];
}

const JSON *Array::get(const unsigned index) const noexcept {
    if(index >= m_data.size()) {
        return nullptr;
    }

    return &m_data[index];  
}

String *Array::getString(const unsigned index, bool &success) noexcept {
    JSON *const json = get(index);
    return json->asString(success);
}

double Array::getFloat64(const unsigned index, bool &success) noexcept {
    JSON *const json = get(index);
    return json->asFloat64(success);
}

std::int64_t Array::getInt64(const unsigned index, bool &success) noexcept {
    JSON *const json = get(index);
    return json->asInt64(success);
}

std::uint64_t Array::getUint64(const unsigned index, bool &success) noexcept {
    JSON *const json = get(index);
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

std::nullptr_t Array::getNull(const unsigned index, bool &success) noexcept {
    JSON *const json = get(index);
    return json->asNull(success);
}

bool Array::getBool(const unsigned index, bool &success) noexcept {
    JSON *const json = get(index);
    return json->asBool(success);
}

bool Array::set(const unsigned index, String &&value) noexcept {
    JSON *const json = get(index);
    if(json == nullptr) {
        return false;
    }

    json->set(std::move(value));
    return true;
}

bool Array::set(const unsigned index, const double value) noexcept {
    JSON *const json = get(index);
    if(json == nullptr) {
        return false;
    }

    json->set(value);
    return true;
}

bool Array::set(const unsigned index, const int64_t value) noexcept {
    JSON *const json = get(index);
    if(json == nullptr) {
        return false;
    }

    json->set(value);
    return true;
}

bool Array::set(const unsigned index, const uint64_t value) noexcept {
    JSON *const json = get(index);
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
    JSON *const json = get(index);
    if(json == nullptr) {
        return false;
    }

    json->set(std::move(value));
    return true;
}

bool Array::set(const unsigned index, Array &&value) noexcept {
    JSON *const json = get(index);
    if(json == nullptr) {
        return false;
    }

    json->set(std::move(value));
    return true;
}

bool Array::set(const unsigned index, std::nullptr_t) noexcept {
    JSON *const json = get(index);
    if(json == nullptr) {
        return false;
    }

    json->set();
    return true;
}

bool Array::set(const unsigned index, const bool value) noexcept {
    JSON *const json = get(index);
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
    if(m_data.size() == 0) {
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
    return get(index);
}

const Array::ValueType *Array::operator[](const unsigned index) const noexcept {
    return get(index);
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
    return m_data.data() + m_data.size();
}

Array::const_iterator Array::end() const noexcept {
    return m_data.data() + m_data.size();
}

}