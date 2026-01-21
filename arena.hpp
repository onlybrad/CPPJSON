#pragma once

#include <cassert>
#include <cstdint>

#include "util.hpp"

namespace CPPJSON {

class Arena;

class ArenaNode {
    friend class Arena;

    ArenaNode *next   = nullptr;
    unsigned   size   = 0U,
               offset = 0U;
    //unsigned char data[];  use getData() to get a pointer to the hidden data

    ArenaNode(unsigned size)               noexcept;
    ArenaNode(const ArenaNode&)                     = delete;
    ArenaNode(ArenaNode&&)                 noexcept = delete;
    ArenaNode &operator=(const ArenaNode&)          = delete;
    ArenaNode &operator=(ArenaNode&&)      noexcept = delete;

    static ArenaNode *create (unsigned size)       noexcept;
    unsigned char    *getData()              const noexcept;
};

class Arena {
    ArenaNode *m_head      = nullptr,
              *m_current   = nullptr;
    unsigned   m_nodeCount = 0U,
               m_nodeMax   = 0U;
#ifndef NDEBUG
    const char *m_name;
#endif

    bool createNextNode(unsigned objectSize);

public:
    static const unsigned MINIMUM_CAPACITY;
    static const unsigned INFINITE_NODES;

    Arena()                                                    noexcept = default;
    ~Arena()                                                   noexcept;
    Arena(const Arena&)                                                 = delete;
    Arena(unsigned size, unsigned maxNodes, const char *name);
    Arena(Arena&&)                                             noexcept = delete;
    Arena &operator=(const Arena&)                                      = delete;
    Arena &operator=(Arena&&)                                  noexcept = delete;

    template<typename T>
    T    *alloc  (unsigned count)                                     noexcept;
    template<typename T>
    bool  reserve(unsigned count)                                     noexcept;
    bool  init   (unsigned size, unsigned maxNodes, const char *name) noexcept;
    void  reset  ()                                                   noexcept;
    char *strdup (const char*, unsigned &length)                      noexcept;
    char *strdup (const char*)                                        noexcept;
};

template<typename T>
bool Arena::reserve(const unsigned count) noexcept {
    assert(count > 0U);

    const Result<unsigned> multResult = Util::safeMult(count, static_cast<unsigned>(sizeof(T)));
    if(!multResult.isSuccess()) {
        return false;
    }
    const unsigned totalSize = multResult.getValue();

    const std::uintptr_t alignment      = static_cast<std::uintptr_t>(alignof(T)),
                         startAddress   = reinterpret_cast<std::uintptr_t>(m_current->getData() + m_current->offset),
                         alignedAddress = (startAddress + (alignment - 1U)) & ~(alignment - 1U);
    unsigned             padding        = static_cast<unsigned>(alignedAddress - startAddress);

    if(m_current->offset + padding + totalSize <= m_current->size) {
        return true;
    }

    return createNextNode(totalSize);
}

template<typename T>
T *Arena::alloc(const unsigned count) noexcept {
    assert(count > 0U);

    const Result<unsigned> multResult = Util::safeMult(count, static_cast<unsigned>(sizeof(T)));
    if(!multResult.isSuccess()) {
        return nullptr;
    }
    const unsigned totalSize = multResult.getValue();

    const std::uintptr_t alignment      = static_cast<std::uintptr_t>(alignof(T));
    const std::uintptr_t startAddress   = reinterpret_cast<std::uintptr_t>(m_current->getData() + m_current->offset);
    std::uintptr_t       alignedAddress = (startAddress + (alignment - 1U)) & ~(alignment - 1U);
    unsigned             padding        = static_cast<unsigned>(alignedAddress - startAddress);

    if(m_current->offset + padding + totalSize > m_current->size) {
        if(!createNextNode(totalSize)) {
            return nullptr;
        }

        alignedAddress = reinterpret_cast<std::uintptr_t>(m_current->getData());
        padding        = 0U;
    }

    m_current->offset += padding + totalSize;

    return reinterpret_cast<T*>(alignedAddress);
}

}