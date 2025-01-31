#include "parser.hpp"

namespace CJSON {

Node *Object::operator[](std::string &key) {
    auto it = m_nodes.find(key);

    if(it == m_nodes.end()) {
        return nullptr;
    }

    return &it->second;
}

Node *Object::get(std::string &key, bool &success) {
    Node *const node = (*this)[key];

    if(node == nullptr) {
        success = false;
        return nullptr;
    }

    success = true;
    return node;
}

Array *Object::getArray(std::string &key, bool &success) {
    Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::ARRAY) {
        success = false;
        return nullptr;
    }

    success = true;
    return &node->m_value.array;
}

Object *Object::getObject(std::string &key, bool &success) {
    Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::OBJECT) {
        success = false;
        return nullptr;
    }

    success = true;
    return &node->m_value.object;
}

int64_t Object::getInt64(std::string &key, bool &success) {
    Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::INT64) {
        success = false;
        return 0;
    }

    success = true;
    return node->m_value.int64;
}

uint64_t Object::getUint64(std::string &key, bool &success) {
    Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::UINT64) {
        success = false;
        return 0U;
    }

    success = true;
    return node->m_value.uint64;
}

double Object::getFloat64(std::string &key, bool &success) {
    Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::FLOAT64) {
        success = false;
        return 0.0;
    }

    success = true;
    return node->m_value.float64;
}

bool Object::getBool(std::string &key, bool &success) {
    Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::BOOL) {
        success = false;
        return false;
    }

    success = true;
    return node->m_value.boolean;
}

std::string *Object::getString(std::string &key, bool &success) {
    Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::STRING) {
        success = false;
        return nullptr;
    }

    success = true;
    return &node->m_value.string;
}

std::nullptr_t Object::getNull(std::string &key, bool &success) {
    Node *const node = (*this)[key];

    success = !(node == nullptr || node->m_type != Type::NULL_T);
    return nullptr;
}

void Object::set(std::string &key, Node &&value) {
    Node *const node = &m_nodes[key];
    node->~Node();

    *node = std::move(value);
}

void Object::set(std::string &key, Array &&value) {
    Node *const node = &m_nodes[key];
    node->~Node();

    node->m_type = Type::ARRAY;
    node->m_value.array.m_nodes = std::move(value.m_nodes);
}

void Object::set(std::string &key, Object &&value) {
    Node *const node = &m_nodes[key];
    node->~Node();

    node->m_type = Type::OBJECT;
    node->m_value.object.m_nodes = std::move(value.m_nodes);
}

void Object::set(std::string &key, const int64_t value) {
    Node *const node = &m_nodes[key];
    node->~Node();

    node->m_type = Type::INT64;
    node->m_value.int64 = value;
}

void Object::set(std::string &key, const uint64_t value) {
    Node *const node = &m_nodes[key];
    node->~Node();

    node->m_type = Type::UINT64;
    node->m_value.uint64 = value;
}

void Object::set(std::string &key, const double value) {
    Node *const node = &m_nodes[key];
    node->~Node();

    node->m_type = Type::FLOAT64;
    node->m_value.float64 = value;
}

void Object::set(std::string &key, const bool value) {
    Node *const node = &m_nodes[key];
    node->~Node();

    node->m_type = Type::BOOL;
    node->m_value.boolean = value;
}

void Object::set(std::string &key, std::string &&value) {
    Node *const node = &m_nodes[key];
    node->~Node();

    node->m_type = Type::STRING;
    node->m_value.string = std::move(value);
}

void Object::set(std::string &key, std::nullptr_t null) {
    Node *const node = &m_nodes[key];
    node->~Node();

    node->m_type = Type::NULL_T;
    node->m_value.null = null;
}

void Object::set(std::string &&key, Node &&value) {
    Node *const node = &m_nodes[std::move(key)];
    node->~Node();

    *node = std::move(value);
}

void Object::set(std::string &&key, Array &&value) {
    Node *const node = &m_nodes[std::move(key)];
    node->~Node();

    node->m_type = Type::ARRAY;
    node->m_value.array.m_nodes = std::move(value.m_nodes);
}

void Object::set(std::string &&key, Object &&value) {
    Node *const node = &m_nodes[std::move(key)];
    node->~Node();

    node->m_type = Type::OBJECT;
    node->m_value.object.m_nodes = std::move(value.m_nodes);
}

void Object::set(std::string &&key, const int64_t value) {
    Node *const node = &m_nodes[std::move(key)];
    node->~Node();

    node->m_type = Type::INT64;
    node->m_value.int64 = value;
}

void Object::set(std::string &&key, const uint64_t value) {
    Node *const node = &m_nodes[std::move(key)];
    node->~Node();

    node->m_type = Type::UINT64;
    node->m_value.uint64 = value;
}

void Object::set(std::string &&key, const double value) {
    Node *const node = &m_nodes[std::move(key)];
    node->~Node();

    node->m_type = Type::FLOAT64;
    node->m_value.float64 = value;
}

void Object::set(std::string &&key, const bool value) {
    Node *const node = &m_nodes[std::move(key)];
    node->~Node();

    node->m_type = Type::BOOL;
    node->m_value.boolean = value;
}

void Object::set(std::string &&key, std::string &&value) {
    Node *const node = &m_nodes[std::move(key)];
    node->~Node();

    node->m_type = Type::STRING;
    node->m_value.string = std::move(value);
}

void Object::set(std::string &&key, std::nullptr_t null) {
    Node *const node = &m_nodes[std::move(key)];
    node->~Node();

    node->m_type = Type::NULL_T;
    node->m_value.null = null;
}

void Object::destroy() {
    m_nodes.~unordered_map();
}

Object::Object() {
    m_nodes.reserve(1 << 3);
}

Object::Object(Object &&other) {
    if(&other != this) {
        m_nodes = std::move(other.m_nodes);
        other.destroy();
    }
}

}