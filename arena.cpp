#include <cstring>
#include <new>

#include "arena.hpp"
#include "memory.hpp"

namespace CPPJSON {

const unsigned Arena::MINIMUM_CAPACITY = 1024U;
const unsigned Arena::INFINITE_NODES   = 0U;

ArenaNode::ArenaNode(const unsigned size_) noexcept : size(size_) {}

ArenaNode *ArenaNode::create(unsigned size) noexcept {
    size = std::max(size, Arena::MINIMUM_CAPACITY);

    void *const nodeAndData = CPPJSON::calloc(sizeof(ArenaNode) + (std::size_t)size, sizeof(unsigned char));
    if(nodeAndData == nullptr) {
        return nullptr;
    }

    return new (nodeAndData) ArenaNode(size);
}

unsigned char *ArenaNode::getData() const noexcept {
    const unsigned char *const data = reinterpret_cast<const unsigned char*>(this + 1);
    return const_cast<unsigned char*>(data);
}

bool Arena::init(const unsigned size, const unsigned nodeMax, const char *const name) noexcept {
    assert(size > 0);

    m_nodeCount = 1U;
    m_nodeMax   = nodeMax;
    m_head      = ArenaNode::create(size);
    m_current   = m_head;

#ifndef NDEBUG
    m_name = name;
#else
    (void)name;
#endif

    return m_head != nullptr;
}

Arena::Arena(unsigned size, unsigned maxNodes, const char *name) {
    if(!init(size, maxNodes, name)) {
        throw std::bad_alloc();
    }
}

Arena::~Arena() noexcept {
    ArenaNode *current = m_head;
    while (current != nullptr) {
        ArenaNode *const next = current->next;
        CPPJSON::free(current);
        current = next;
    }
}

void Arena::reset() noexcept {  
    m_current      = m_head;
    m_head->offset = 0U;
}

char *Arena::strdup(const char *const str, unsigned &length) noexcept {
    assert(str != nullptr);

    const std::size_t sizetLength = std::strlen(str);
    if(sizetLength >= std::size_t(std::numeric_limits<unsigned>::max())) {
        length = 0U;
        return nullptr;
    }

    char *const copy = static_cast<char*>(alloc<char>(unsigned(sizetLength) + 1U));
    if(copy == nullptr) {
        length = 0U;
        return nullptr;
    }

    length = unsigned(sizetLength);
    return std::strcpy(copy, str);
}

char *Arena::strdup(const char *const str) noexcept {
    assert(str != nullptr);

    unsigned length;
    return strdup(str, length);
}

bool Arena::createNextNode(const unsigned objectSize) {
    assert(objectSize > 0U);

    unsigned nodeSize = m_current->size;
    if(nodeSize < objectSize) {
        if(nodeSize > unsigned(std::numeric_limits<decltype(nodeSize)>::max()) / 2U) {
            nodeSize = objectSize;
        } else do {
            nodeSize *= 2U;
        } while(nodeSize < objectSize);
    }
    
    ArenaNode *const next = m_current->next;
    if(next == nullptr) {
        if(m_nodeCount == m_nodeMax) {
            return false;
        }

        if((m_current->next = ArenaNode::create(nodeSize)) != nullptr) {
            m_current = m_current->next;
            m_nodeCount++;
            return true;
        }

        return false;
    }

    if(next->size < objectSize) {
        ArenaNode *const nextNext = next->next;
        CPPJSON::free(next);

        if((m_current->next = ArenaNode::create(nodeSize)) != nullptr) {
            m_current->next->next = nextNext;
            m_current = m_current->next;
            return true;
        }

        m_current->next = nextNext;
        m_nodeCount--;
        return false;
    }

    return true;
}

}