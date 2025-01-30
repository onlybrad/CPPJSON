#include "parser.hpp"

namespace CJSON {

JSON_Node *Array::get(const unsigned int index, bool &success) {
    if(index >= m_data.size()) {
        success = false;
        return nullptr;
    }
    success = true;
    return &m_data[index];
}

Array *Array::getArray(const unsigned int index, bool &success) {
    if(index >= m_data.size() || m_data[index].m_type == Type::ARRAY) {
        success = false;
        return nullptr;
    }
    success = true;
    return &m_data[index].m_value.array;
}

Object *Array::getObject(const unsigned int index, bool &success) {
    if(index >= m_data.size() || m_data[index].m_type == Type::OBJECT) {
        success = false;
        return nullptr;
    }
    success = true;
    return &m_data[index].m_value.object;
}

int64_t Array::getInt64(const unsigned int index, bool &success) {
    if(index >= m_data.size() || m_data[index].m_type == Type::INT64) {
        success = false;
        return 0;
    }
    success = true;
    return m_data[index].m_value.int64;
}

uint64_t Array::getUint64(const unsigned int index, bool &success) {
    if(index >= m_data.size() || m_data[index].m_type == Type::UINT64) {
        success = false;
        return 0U;
    }
    success = true;
    return m_data[index].m_value.uint64;
}

double Array::getFloat64(const unsigned int index, bool &success) {
    if(index >= m_data.size() || m_data[index].m_type == Type::FLOAT64) {
        success = false;
        return 0.0;
    }
    success = true;
    return m_data[index].m_value.float64;
}

bool Array::getBool(const unsigned int index, bool &success) {
    if(index >= m_data.size() || m_data[index].m_type == Type::BOOL) {
        success = false;
        return false;
    }
    success = true;
    return m_data[index].m_value.boolean;
}

std::string *Array::getString(const unsigned int index, bool &success) {
        if(index >= m_data.size() || m_data[index].m_type == Type::STRING) {
        success = false;
        return nullptr;
    }
    success = true;
    return &m_data[index].m_value.string;
}

nullptr_t Array::getNull(const unsigned int index, bool &success) {
    success = !(index >= m_data.size() || m_data[index].m_type == Type::STRING);
    return nullptr;
}

void Array::set(const unsigned int index, JSON_Node &&value) {
    if(index >= m_data.size() || (value.m_type == Type::ARRAY && &value.m_value.array.m_data == &m_data)) {
        return;
    }
    m_data[index].~JSON_Node();
    m_data[index] = std::move(value);
}

void Array::set(const unsigned int index, Array &&value) {
    if(index >= m_data.size() || &value == this) {
        return;
    }
    m_data[index].~JSON_Node();
    m_data[index].m_type = Type::ARRAY;
    m_data[index].m_value.array.m_data = std::move(value.m_data);
}

void Array::set(const unsigned int index, Object &&value) {
    if(index >= m_data.size()) {
        return;
    }
    m_data[index].~JSON_Node();
    m_data[index].m_type = Type::OBJECT;
    m_data[index].m_value.object.m_data = std::move(value.m_data);
}

void Array::set(const unsigned int index, const int64_t value) {
    if(index >= m_data.size()) {
        return;
    }
    m_data[index].~JSON_Node();
    m_data[index].m_type = Type::INT64;
    m_data[index].m_value.int64 = value;
}

void Array::set(const unsigned int index, const uint64_t value) {
    if(index >= m_data.size()) {
        return;
    }
    m_data[index].~JSON_Node();
    m_data[index].m_type = Type::UINT64;
    m_data[index].m_value.uint64 = value;
}

void Array::set(const unsigned int index, const double value) {
    if(index >= m_data.size()) {
        return;
    }
    m_data[index].~JSON_Node();
    m_data[index].m_type = Type::FLOAT64;
    m_data[index].m_value.float64 = value; 
}

void Array::set(const unsigned int index, const bool value) {
    if(index >= m_data.size()) {
        return;
    }
    m_data[index].~JSON_Node();
    m_data[index].m_type = Type::BOOL;
    m_data[index].m_value.boolean = value; 
}

void Array::set(const unsigned int index, std::string &&value) {
    if(index >= m_data.size()) {
        return;
    }
    m_data[index].~JSON_Node();
    m_data[index].m_type = Type::STRING;
    m_data[index].m_value.string = std::move(value); 
}

void Array::set(const unsigned int index, std::nullptr_t null) {
    if(index >= m_data.size()) {
        return;
    }
    m_data[index].~JSON_Node();
    m_data[index].m_type = Type::NULL_T;
    m_data[index].m_value.null = null;
}

void Array::push_back(Array &&value) {
    m_data.emplace_back();
    JSON_Node &node = m_data.back();
    node.m_type = Type::ARRAY;
    node.m_value.array.m_data = std::move(value.m_data);
}

void Array::push_back(Object &&value) {
    m_data.emplace_back();
    JSON_Node &node = m_data.back();
    node.m_type = Type::OBJECT;
    node.m_value.object.m_data = std::move(value.m_data);
}

void Array::push_back(const int64_t value) {
    m_data.emplace_back();
    JSON_Node &node = m_data.back();
    node.m_type = Type::INT64;
    node.m_value.int64 = value;
}

void Array::push_back(const uint64_t value) {
    m_data.emplace_back();
    JSON_Node &node = m_data.back();
    node.m_type = Type::UINT64;
    node.m_value.uint64 = value;
}

void Array::push_back(const double value) {
    m_data.emplace_back();
    JSON_Node &node = m_data.back();
    node.m_type = Type::FLOAT64;
    node.m_value.float64 = value;
}

void Array::push_back(const bool value) {
    m_data.emplace_back();
    JSON_Node &node = m_data.back();
    node.m_type = Type::BOOL;
    node.m_value.boolean = value;
}

void Array::push_back(std::string &&value) {
    m_data.emplace_back();
    JSON_Node &node = m_data.back();
    node.m_type = Type::STRING;
    node.m_value.string = std::move(value);
}

void Array::push_back(std::nullptr_t null) {
    m_data.emplace_back();
    JSON_Node &node = m_data.back();
    node.m_type = Type::NULL_T;
    node.m_value.null = null;
}

unsigned int Array::size() {
    return (unsigned int)m_data.size();
}

void Array::destroy() {
    m_data.~vector();
}

Array::Array() {
    m_data.reserve(1 << 3);
}

Array::Array(Array &&other) {
    if(&other != this) {
        m_data = std::move(other.m_data);
        other.destroy();
    }
}

}