#include <cstring>

#include "string.hpp"
#include "parser.hpp"

namespace CPPJSON {

const unsigned String::MINIMUM_CAPACITY = 8U;

String::String(const Allocator &allocator) :
m_data(allocator) 
{}

String::String(Allocator &&allocator) noexcept :
m_data(std::move(allocator)) 
{}

String::~String() noexcept {}

String::ValueType &String::operator[](const unsigned index) noexcept {
    return m_data[index];
}

const String::ValueType &String::operator[](const unsigned index) const noexcept {
    return m_data[index];
}

const String &String::operator+=(const std::string &str) {
    return operator+=(str.c_str());
}

const String &String::operator=(const std::string &str) {
    return operator=(str.c_str());
}

bool String::operator==(const std::string &str) const {
    return operator==(str.c_str());
}

const String &String::operator+=(const String &str) {
    return operator+=(str.getData());
}

String &String::operator=(const String &str) {
    operator=(str.getData());

    return *this;
}

String &String::operator=(String &&str) noexcept {
    if(this != &str) {
        operator=(std::move(str.getData()));
    }

    return *this;
}

bool String::operator==(const String &str) const {
    return operator==(str.getData());
}

const String &String::operator+=(const char *const str) {
    assert(str != nullptr);

    m_data.operator+=(str);

    return *this;
}

const String &String::operator=(const char *const str) {
    assert(str != nullptr);

    m_data.operator=(str);

    return *this;
}

bool String::operator==(const char *const str) const {
    assert(str != nullptr);

    return std::strcmp(m_data.c_str(), str) == 0;
}

unsigned String::size() const noexcept {
    return static_cast<unsigned>(m_data.size());
}

bool String::reserve(unsigned capacity) {
    if(capacity < String::MINIMUM_CAPACITY) {
        capacity = String::MINIMUM_CAPACITY;
    }

    try {
        m_data.reserve(static_cast<std::size_t>(capacity));
    } catch (...) {
        return false;
    }

    return true;
}

void String::push(const ValueType value) {
    m_data.push_back(value);
}

const char *String::getData() const noexcept { 
    return m_data.c_str(); 
}

String::Container &String::getContainer() noexcept {
    return m_data;
}

String::Allocator String::getAllocator() const noexcept { 
    return m_data.get_allocator();
}

}