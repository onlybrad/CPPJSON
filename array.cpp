#include "parser.hpp"

namespace CJSON {

Node *Array::operator[](const unsigned int index) {
    if(index >= m_nodes.size()) {
        return nullptr;
    }
    return &m_nodes[index];
}

Array &Array::operator=(Array &&other) {
    if(&other != this) {
        m_nodes = std::move(other.m_nodes);
        other.destroy();
    }

    return *this;
}  

Node *Array::get(const unsigned int index, bool &success) {
    if(index >= m_nodes.size()) {
        success = false;
        return nullptr;
    }
    success = true;
    return &m_nodes[index];
}

Array *Array::getArray(const unsigned int index, bool &success) {
    if(index >= m_nodes.size() || m_nodes[index].m_type == Type::ARRAY) {
        success = false;
        return nullptr;
    }
    success = true;
    return &m_nodes[index].m_value.array;
}

Object *Array::getObject(const unsigned int index, bool &success) {
    if(index >= m_nodes.size() || m_nodes[index].m_type == Type::OBJECT) {
        success = false;
        return nullptr;
    }
    success = true;
    return &m_nodes[index].m_value.object;
}

int64_t Array::getInt64(const unsigned int index, bool &success) {
    if(index >= m_nodes.size() || m_nodes[index].m_type == Type::INT64) {
        success = false;
        return 0;
    }
    success = true;
    return m_nodes[index].m_value.int64;
}

uint64_t Array::getUint64(const unsigned int index, bool &success) {
    if(index >= m_nodes.size() || m_nodes[index].m_type == Type::UINT64) {
        success = false;
        return 0U;
    }
    success = true;
    return m_nodes[index].m_value.uint64;
}

double Array::getFloat64(const unsigned int index, bool &success) {
    if(index >= m_nodes.size() || m_nodes[index].m_type == Type::FLOAT64) {
        success = false;
        return 0.0;
    }
    success = true;
    return m_nodes[index].m_value.float64;
}

bool Array::getBool(const unsigned int index, bool &success) {
    if(index >= m_nodes.size() || m_nodes[index].m_type == Type::BOOL) {
        success = false;
        return false;
    }
    success = true;
    return m_nodes[index].m_value.boolean;
}

std::string *Array::getString(const unsigned int index, bool &success) {
    if(index >= m_nodes.size() || m_nodes[index].m_type == Type::STRING) {
        success = false;
        return nullptr;
    }
    success = true;
    return &m_nodes[index].m_value.string;
}

nullptr_t Array::getNull(const unsigned int index, bool &success) {
    success = !(index >= m_nodes.size() || m_nodes[index].m_type == Type::STRING);
    return nullptr;
}

void Array::set(const unsigned int index, Node &&value) {
    if(value.m_type == Type::ARRAY && &value.m_value.array == this) {
        return;
    }

    if(index >= m_nodes.size()) {
        m_nodes.resize(index + 1U);
    }

    m_nodes[index].destroy();
    m_nodes[index] = std::move(value);
}

void Array::set(const unsigned int index, Array &&value) {
    if(&value == this) {
        return;
    }

    if(index >= m_nodes.size()) {
        m_nodes.resize(index + 1U);
    }

    m_nodes[index].destroy();
    m_nodes[index].m_type = Type::ARRAY;
    m_nodes[index].m_value.array = std::move(value);
}

void Array::set(const unsigned int index, Object &&value) {
    if(index >= m_nodes.size()) {
        m_nodes.resize(index + 1U);
    }
    m_nodes[index].destroy();
    m_nodes[index].m_type = Type::OBJECT;
    m_nodes[index].m_value.object = std::move(value);
}

void Array::set(const unsigned int index, const int64_t value) {
    if(index >= m_nodes.size()) {
        m_nodes.resize(index + 1U);
    }
    m_nodes[index].destroy();
    m_nodes[index].m_type = Type::INT64;
    m_nodes[index].m_value.int64 = value;
}

void Array::set(const unsigned int index, const uint64_t value) {
    if(index >= m_nodes.size()) {
        m_nodes.resize(index + 1U);
    }
    m_nodes[index].destroy();
    m_nodes[index].m_type = Type::UINT64;
    m_nodes[index].m_value.uint64 = value;
}

void Array::set(const unsigned int index, const double value) {
    if(index >= m_nodes.size()) {
        m_nodes.resize(index + 1U);
    }
    m_nodes[index].destroy();
    m_nodes[index].m_type = Type::FLOAT64;
    m_nodes[index].m_value.float64 = value; 
}

void Array::set(const unsigned int index, const bool value) {
    if(index >= m_nodes.size()) {
        m_nodes.resize(index + 1U);
    }
    m_nodes[index].destroy();
    m_nodes[index].m_type = Type::BOOL;
    m_nodes[index].m_value.boolean = value; 
}

void Array::set(const unsigned int index, std::string &&value) {
    if(index >= m_nodes.size()) {
        m_nodes.resize(index + 1U);
    }
    m_nodes[index].destroy();
    m_nodes[index].m_type = Type::STRING;
    m_nodes[index].m_value.string = std::move(value); 
}

void Array::set(const unsigned int index, std::nullptr_t null) {
    if(index >= m_nodes.size()) {
        m_nodes.resize(index + 1U);
    }
    m_nodes[index].destroy();
    m_nodes[index].m_type = Type::NULL_T;
    m_nodes[index].m_value.null = null;
}

void Array::push_back(Array &&value) {
    if(m_nodes.size() == (size_t)UINT_MAX) {
        return;
    }
    m_nodes.emplace_back();
    Node &node = m_nodes.back();
    node.m_type = Type::ARRAY;
    node.m_value.array = std::move(value);
}

void Array::push_back(Object &&value) {
    if(m_nodes.size() == (size_t)UINT_MAX) {
        return;
    }
    m_nodes.emplace_back();
    Node &node = m_nodes.back();
    node.m_type = Type::OBJECT;
    node.m_value.object = std::move(value);
}

void Array::push_back(const int64_t value) {
    if(m_nodes.size() == (size_t)UINT_MAX) {
        return;
    }
    m_nodes.emplace_back();
    Node &node = m_nodes.back();
    node.m_type = Type::INT64;
    node.m_value.int64 = value;
}

void Array::push_back(const uint64_t value) {
    if(m_nodes.size() == (size_t)UINT_MAX) {
        return;
    }
    m_nodes.emplace_back();
    Node &node = m_nodes.back();
    node.m_type = Type::UINT64;
    node.m_value.uint64 = value;
}

void Array::push_back(const double value) {
    if(m_nodes.size() == (size_t)UINT_MAX) {
        return;
    }
    m_nodes.emplace_back();
    Node &node = m_nodes.back();
    node.m_type = Type::FLOAT64;
    node.m_value.float64 = value;
}

void Array::push_back(const bool value) {
    if(m_nodes.size() == (size_t)UINT_MAX) {
        return;
    }
    m_nodes.emplace_back();
    Node &node = m_nodes.back();
    node.m_type = Type::BOOL;
    node.m_value.boolean = value;
}

void Array::push_back(std::string &&value) {
    if(m_nodes.size() == (size_t)UINT_MAX) {
        return;
    }
    m_nodes.emplace_back();
    Node &node = m_nodes.back();
    node.m_type = Type::STRING;
    node.m_value.string = std::move(value);
}

void Array::push_back(std::nullptr_t null) {
    if(m_nodes.size() == (size_t)UINT_MAX) {
        return;
    }
    m_nodes.emplace_back();
    Node &node = m_nodes.back();
    node.m_type = Type::NULL_T;
    node.m_value.null = null;
}

unsigned int Array::size() {
    return (unsigned int)m_nodes.size();
}

void Array::destroy() {
    m_nodes.~vector();
}

Array::Array() {
    m_nodes.reserve(1 << 3);
}

Array::Array(Array &&other) {
    m_nodes = std::move(other.m_nodes);
    other.destroy();
}

}