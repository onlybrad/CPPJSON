#include "parser.hpp"

namespace CJSON {

Query::Query(Node *const node) : m_node(node) {}

Query Query::operator[](const unsigned int index) {
    if(m_node == nullptr || m_node->type() != Type::ARRAY) {
        m_node = nullptr;
    } else {
        bool success;
        m_node = m_node->value().array.get(index, success);
        if(!success) {
            m_node = nullptr;
        }
    }

    return *this;
}

Query Query::operator[](const std::string &key) {
    if(m_node == nullptr || m_node->type() != Type::OBJECT) {
        m_node = nullptr;
    } else {
        bool success;
        m_node = m_node->value().object.get(key, success);
        if(!success) {
            m_node = nullptr;
        }
    }

    return *this;
}

Node *Query::get() {
    return m_node;
}

Array *Query::getArray(bool &success) {
    success = m_node != nullptr && m_node->type() == Type::ARRAY;

    return m_node == nullptr ? nullptr : &m_node->value().array;
}

Object *Query::getObject(bool &success) {
    success = m_node != nullptr && m_node->type() == Type::OBJECT;

    return m_node == nullptr ? nullptr : &m_node->value().object;
}

int64_t Query::getInt64(bool &success) {
    success = m_node != nullptr && m_node->type() == Type::INT64;

    return m_node == nullptr ? 0 : m_node->value().int64;
}

uint64_t Query::getUint64(bool &success) {
    success = m_node != nullptr && m_node->type() == Type::UINT64;

    return m_node == nullptr ? 0U : m_node->value().uint64;
}

double Query::getFloat64(bool &success) {
    success = m_node != nullptr && m_node->type() == Type::FLOAT64;

    return m_node == nullptr ? 0.0 : m_node->value().float64;  
}

bool Query::getBool(bool &success) {
    success = m_node != nullptr && m_node->type() == Type::BOOL;

    return m_node == nullptr ? false : m_node->value().boolean;  
}

std::string *Query::getString(bool &success) {
    success = m_node != nullptr && m_node->type() == Type::STRING;

    return m_node == nullptr ? nullptr : &m_node->value().string;  
}

std::nullptr_t Query::getNull(bool &success) {
    success = m_node != nullptr && m_node->type() == Type::NULL_T;

    return nullptr;
}

}
