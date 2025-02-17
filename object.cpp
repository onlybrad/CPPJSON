#include "parser.hpp"

#define DEFAULT_CAPACITY 1 << 3

namespace CJSON {

Object &Object::operator=(Object &&other) {
    if(&other != this) {
        m_nodes = std::move(other.m_nodes);
        other.destroy();
    }

    return *this;
}

JSON *Object::operator[](const std::string &key) {
    auto it = m_nodes.find(key);

    if(it == m_nodes.end()) {
        return nullptr;
    }

    return &it->second;
}

JSON *Object::get(const std::string &key, bool &success) {
    JSON *const node = (*this)[key];

    if(node == nullptr) {
        success = false;
        return nullptr;
    }

    success = true;
    return node;
}

Array *Object::getArray(const std::string &key, bool &success) {
    JSON *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::ARRAY) {
        success = false;
        return nullptr;
    }

    success = true;
    return &node->m_data.array;
}

Object *Object::getObject(const std::string &key, bool &success) {
    JSON *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::OBJECT) {
        success = false;
        return nullptr;
    }

    success = true;
    return &node->m_data.object;
}

int64_t Object::getInt64(const std::string &key, bool &success) {
    JSON *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::INT64) {
        success = false;
        return 0;
    }

    success = true;
    return node->m_data.int64;
}

uint64_t Object::getUint64(const std::string &key, bool &success) {
    JSON *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::UINT64) {
        success = false;
        return 0U;
    }

    success = true;
    return node->m_data.uint64;
}

double Object::getFloat64(const std::string &key, bool &success) {
    JSON *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::FLOAT64) {
        success = false;
        return 0.0;
    }

    success = true;
    return node->m_data.float64;
}

bool Object::getBool(const std::string &key, bool &success) {
    JSON *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::BOOL) {
        success = false;
        return false;
    }

    success = true;
    return node->m_data.boolean;
}

std::string *Object::getString(const std::string &key, bool &success) {
    JSON *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::STRING) {
        success = false;
        return nullptr;
    }

    success = true;
    return &node->m_data.string;
}

std::nullptr_t Object::getNull(const std::string &key, bool &success) {
    JSON *const node = (*this)[key];

    success = !(node == nullptr || node->m_type != Type::NULL_T);
    return nullptr;
}

void Object::set(const std::string &key, JSON &&value) {
    JSON *const node = &m_nodes[key];
    node->destroy();

    *node = std::move(value);
}

void Object::set(const std::string &key, Array &&value) {
    JSON &node = m_nodes[key];
    node.destroy();

    node.m_type = Type::ARRAY;
    node.m_data.array = std::move(value);
}

void Object::set(const std::string &key, Object &&value) {
    JSON &node = m_nodes[key];
    node.destroy();

    node.m_type = Type::OBJECT;
    node.m_data.object = std::move(value);
}

void Object::set(const std::string &key, const int64_t value) {
    JSON &node = m_nodes[key];
    node.destroy();

    node.m_type = Type::INT64;
    node.m_data.int64 = value;
}

void Object::set(const std::string &key, const uint64_t value) {
    JSON &node = m_nodes[key];
    node.destroy();

    node.m_type = Type::UINT64;
    node.m_data.uint64 = value;
}

void Object::set(const std::string &key, const double value) {
    JSON &node = m_nodes[key];
    node.destroy();

    node.m_type = Type::FLOAT64;
    node.m_data.float64 = value;
}

void Object::set(const std::string &key, const bool value) {
    JSON &node = m_nodes[key];
    node.destroy();

    node.m_type = Type::BOOL;
    node.m_data.boolean = value;
}

void Object::set(const std::string &key, std::string &&value) {
    JSON &node = m_nodes[key];
    node.destroy();

    node.m_type = Type::STRING;
    node.m_data.string = std::move(value);
}

void Object::set(const std::string &key, std::nullptr_t null) {
    JSON &node = m_nodes[key];
    node.destroy();

    node.m_type = Type::NULL_T;
    node.m_data.null = null;
}

void Object::set(std::string &&key, JSON &&value) {
    JSON *const node = &m_nodes[std::move(key)];
    node->destroy();

    *node = std::move(value);
}

void Object::set(std::string &&key, Array &&value) {
    JSON &node = m_nodes[std::move(key)];
    node.destroy();

    node.m_type = Type::ARRAY;
    node.m_data.array = std::move(value);
}

void Object::set(std::string &&key, Object &&value) {
    JSON &node = m_nodes[std::move(key)];
    node.destroy();

    node.m_type = Type::OBJECT;
    node.m_data.object = std::move(value);
}

void Object::set(std::string &&key, const int64_t value) {
    JSON &node = m_nodes[std::move(key)];
    node.destroy();

    node.m_type = Type::INT64;
    node.m_data.int64 = value;
}

void Object::set(std::string &&key, const uint64_t value) {
    JSON &node = m_nodes[std::move(key)];
    node.destroy();

    node.m_type = Type::UINT64;
    node.m_data.uint64 = value;
}

void Object::set(std::string &&key, const double value) {
    JSON &node = m_nodes[std::move(key)];
    node.destroy();

    node.m_type = Type::FLOAT64;
    node.m_data.float64 = value;
}

void Object::set(std::string &&key, const bool value) {
    JSON &node = m_nodes[std::move(key)];
    node.destroy();

    node.m_type = Type::BOOL;
    node.m_data.boolean = value;
}

void Object::set(std::string &&key, std::string &&value) {
    JSON &node = m_nodes[std::move(key)];
    node.destroy();

    node.m_type = Type::STRING;
    node.m_data.string = std::move(value);
}

void Object::set(std::string &&key, std::nullptr_t null) {
    JSON &node = m_nodes[std::move(key)];
    node.destroy();

    node.m_type = Type::NULL_T;
    node.m_data.null = null;
}

void Object::destroy() {
    m_nodes.~unordered_map();
}

Object::Object() {
    m_nodes.reserve(DEFAULT_CAPACITY);
}

Object::Object(Object &&other) {
    m_nodes = std::move(other.m_nodes);
    other.destroy();
}

}