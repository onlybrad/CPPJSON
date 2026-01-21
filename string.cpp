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

String::String(const String &string) :
m_data(string.m_data)
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
    return operator+=(str.getCString());
}

String &String::operator=(const String &str) {
    operator=(str.getCString());

    return *this;
}

String &String::operator=(String &&str) noexcept {
    if(this != &str) {
        m_data = std::move(str.m_data);
    }

    return *this;
}

bool String::operator==(const String &str) const {
    return this == &str || (*this)==(str.getCString());
}

const String &String::operator+=(const char *const str) {
    assert(str != nullptr);

    m_data += str;

    return *this;
}

const String &String::operator=(const char *const str) {
    assert(str != nullptr);

    m_data = str;

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

const char *String::getCString() const noexcept { 
    return m_data.c_str(); 
}

String::Allocator String::getAllocator() const noexcept { 
    return m_data.get_allocator();
}

void String::destructor() noexcept {
    m_data.~Container();
}

String::const_iterator String::begin() const noexcept {
    return m_data.begin();
}

String::const_iterator String::end() const noexcept {
    return m_data.end();
}

}