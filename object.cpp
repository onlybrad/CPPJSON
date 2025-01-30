#include "parser.hpp"

namespace CJSON {

JSON_Node *Object::operator[](std::string &key) {
    auto it = m_data.find(key);

    if(it == m_data.end()) {
        return nullptr;
    }

    return &it->second;
}

JSON_Node *Object::get(std::string &key, bool &success) {
    JSON_Node *const node = (*this)[key];

    if(node == nullptr) {
        success = false;
        return nullptr;
    }

    success = true;
    return node;
}

Array *Object::getArray(std::string &key, bool &success) {
    JSON_Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::ARRAY) {
        success = false;
        return nullptr;
    }

    success = true;
    return &node->m_value.array;
}

Object *Object::getObject(std::string &key, bool &success) {
    JSON_Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::OBJECT) {
        success = false;
        return nullptr;
    }

    success = true;
    return &node->m_value.object;
}

int64_t Object::getInt64(std::string &key, bool &success) {
    JSON_Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::INT64) {
        success = false;
        return 0;
    }

    success = true;
    return node->m_value.int64;
}

uint64_t Object::getUint64(std::string &key, bool &success) {
    JSON_Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::UINT64) {
        success = false;
        return 0U;
    }

    success = true;
    return node->m_value.uint64;
}

double Object::getFloat64(std::string &key, bool &success) {
    JSON_Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::FLOAT64) {
        success = false;
        return 0.0;
    }

    success = true;
    return node->m_value.float64;
}

bool Object::getBool(std::string &key, bool &success) {
    JSON_Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::BOOL) {
        success = false;
        return false;
    }

    success = true;
    return node->m_value.boolean;
}

std::string *Object::getString(std::string &key, bool &success) {
    JSON_Node *const node = (*this)[key];

    if(node == nullptr || node->m_type != Type::STRING) {
        success = false;
        return nullptr;
    }

    success = true;
    return &node->m_value.string;
}

std::nullptr_t Object::getNull(std::string &key, bool &success) {
    JSON_Node *const node = (*this)[key];

    success = !(node == nullptr || node->m_type != Type::NULL_T);
    return nullptr;
}

void Object::set(std::string &key, JSON_Node &&value) {
    JSON_Node *const node = &m_data[key];
    node->~JSON_Node();

    *node = std::move(value);
}

void Object::set(std::string &key, Array &&value) {
    JSON_Node *const node = &m_data[key];
    node->~JSON_Node();

    node->m_type = Type::ARRAY;
    node->m_value.array.m_data = std::move(value.m_data);
}

void Object::set(std::string &key, Object &&value) {
    JSON_Node *const node = &m_data[key];
    node->~JSON_Node();

    node->m_type = Type::OBJECT;
    node->m_value.object.m_data = std::move(value.m_data);
}

void Object::set(std::string &key, const int64_t value) {
    JSON_Node *const node = &m_data[key];
    node->~JSON_Node();

    node->m_type = Type::INT64;
    node->m_value.int64 = value;
}

void Object::set(std::string &key, const uint64_t value) {
    JSON_Node *const node = &m_data[key];
    node->~JSON_Node();

    node->m_type = Type::UINT64;
    node->m_value.uint64 = value;
}

void Object::set(std::string &key, const double value) {
    JSON_Node *const node = &m_data[key];
    node->~JSON_Node();

    node->m_type = Type::FLOAT64;
    node->m_value.float64 = value;
}

void Object::set(std::string &key, const bool value) {
    JSON_Node *const node = &m_data[key];
    node->~JSON_Node();

    node->m_type = Type::BOOL;
    node->m_value.boolean = value;
}

void Object::set(std::string &key, std::string &&value) {
    JSON_Node *const node = &m_data[key];
    node->~JSON_Node();

    node->m_type = Type::STRING;
    node->m_value.string = std::move(value);
}

void Object::set(std::string &key, std::nullptr_t null) {
    JSON_Node *const node = &m_data[key];
    node->~JSON_Node();

    node->m_type = Type::NULL_T;
    node->m_value.null = null;
}

void Object::set(std::string &&key, JSON_Node &&value) {
    JSON_Node *const node = &m_data[std::move(key)];
    node->~JSON_Node();

    *node = std::move(value);
}

void Object::set(std::string &&key, Array &&value) {
    JSON_Node *const node = &m_data[std::move(key)];
    node->~JSON_Node();

    node->m_type = Type::ARRAY;
    node->m_value.array.m_data = std::move(value.m_data);
}

void Object::set(std::string &&key, Object &&value) {
    JSON_Node *const node = &m_data[std::move(key)];
    node->~JSON_Node();

    node->m_type = Type::OBJECT;
    node->m_value.object.m_data = std::move(value.m_data);
}

void Object::set(std::string &&key, const int64_t value) {
    JSON_Node *const node = &m_data[std::move(key)];
    node->~JSON_Node();

    node->m_type = Type::INT64;
    node->m_value.int64 = value;
}

void Object::set(std::string &&key, const uint64_t value) {
    JSON_Node *const node = &m_data[std::move(key)];
    node->~JSON_Node();

    node->m_type = Type::UINT64;
    node->m_value.uint64 = value;
}

void Object::set(std::string &&key, const double value) {
    JSON_Node *const node = &m_data[std::move(key)];
    node->~JSON_Node();

    node->m_type = Type::FLOAT64;
    node->m_value.float64 = value;
}

void Object::set(std::string &&key, const bool value) {
    JSON_Node *const node = &m_data[std::move(key)];
    node->~JSON_Node();

    node->m_type = Type::BOOL;
    node->m_value.boolean = value;
}

void Object::set(std::string &&key, std::string &&value) {
    JSON_Node *const node = &m_data[std::move(key)];
    node->~JSON_Node();

    node->m_type = Type::STRING;
    node->m_value.string = std::move(value);
}

void Object::set(std::string &&key, std::nullptr_t null) {
    JSON_Node *const node = &m_data[std::move(key)];
    node->~JSON_Node();

    node->m_type = Type::NULL_T;
    node->m_value.null = null;
}

void Object::destroy() {
    m_data.~unordered_map();
}

Object::Object() {
    m_data.reserve(1 << 3);
}

Object::Object(Object &&other) {
    if(&other != this) {
        m_data = std::move(other.m_data);
        other.destroy();
    }
}

}