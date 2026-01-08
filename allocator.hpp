#pragma once

#include <cassert>
#include <cstdint>
#include <limits>

#include "arena.hpp"
#include "memory.hpp"

namespace CPPJSON {

template<typename T>
struct GeneralAllocator {
    typedef T                value_type;
    typedef T               *pointer;
    typedef const T         *const_pointer;
    typedef T               &reference;
    typedef const T         &const_reference;
    typedef std::size_t      size_type;
    typedef std::ptrdiff_t   difference_type;
    typedef std::true_type   propagate_on_container_copy_assignment;
    typedef std::true_type   propagate_on_container_move_assignment;
    typedef std::true_type   propagate_on_container_swap;
    typedef std::true_type   is_always_equal;

    GeneralAllocator()                        noexcept = default;
    GeneralAllocator(const GeneralAllocator&) noexcept {};

    template<typename U, typename... Args>
    void construct(U *const ptr, Args&&... args) {
        new (ptr) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* ptr) noexcept {
        ptr->~U();
    }

    T   *allocate  (size_type count);
    void deallocate(T *data, size_type count) noexcept;

    static GeneralAllocator<T> &getDefault()                   noexcept;
    static T                   *s_allocate  (size_type count);
    static void                 s_deallocate(T *data)          noexcept;

    bool operator==(const GeneralAllocator &generalAllocator) const noexcept {
        return true;
    }

    bool operator!=(const GeneralAllocator& generalAllocator) const noexcept {
        return false;
    }

private:
    static GeneralAllocator<T> defaultAllocator;
};

template<typename T>
T *GeneralAllocator<T>::allocate(const size_type count) {
    if(count == 0U) {
        return nullptr;
    }

    if(count > std::numeric_limits<size_type>::max() / static_cast<size_type>(sizeof(T))) {
        throw std::bad_array_new_length();
    }

    T *const data = static_cast<T*>(malloc(static_cast<std::size_t>(count) * sizeof(T)));
    if(data == nullptr) {
        throw std::bad_alloc();
    }

    return data;
}

template<typename T>
void GeneralAllocator<T>::deallocate(T *const data, const size_type) noexcept {
    free(data);
}

template<typename T>
GeneralAllocator<T> &GeneralAllocator<T>::getDefault() noexcept {
    return defaultAllocator;
}

template<typename T>
T *GeneralAllocator<T>::s_allocate(size_type count) {
    return getDefault().allocate(count);
}

template<typename T>
void GeneralAllocator<T>::s_deallocate(T *data) noexcept {
    getDefault().deallocate(data, 0U);
}

template<typename T>
struct ArenaAllocator {    
    typedef T                value_type;
    typedef T               *pointer;
    typedef const T         *const_pointer;
    typedef T               &reference;
    typedef const T         &const_reference;
    typedef std::size_t      size_type;
    typedef std::ptrdiff_t   difference_type;
    typedef std::true_type   propagate_on_container_copy_assignment;
    typedef std::true_type   propagate_on_container_move_assignment;
    typedef std::true_type   propagate_on_container_swap;
    typedef std::false_type  is_always_equal;
    
    ArenaAllocator()                      noexcept = default;
    ArenaAllocator(Arena*)                noexcept;

    template<typename U>
    ArenaAllocator(const ArenaAllocator<U>&) noexcept;
    
    template<typename U, typename... Args>
    void construct(U *const ptr, Args&&... args) {
        new (ptr) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* ptr) noexcept {
        ptr->~U();
    }

    T   *allocate  (size_type count);
    void deallocate(T *data, size_type count) noexcept;

    bool operator==(const ArenaAllocator &arenaAllocator) const noexcept {
        return m_arena == arenaAllocator.m_arena;
    }

    bool operator!=(const ArenaAllocator& arenaAllocator) const noexcept {
        return m_arena != arenaAllocator.m_arena;
    }

private:
    template<typename U> friend struct ArenaAllocator;
    Arena *m_arena = nullptr;
};

template<typename T>
ArenaAllocator<T>::ArenaAllocator(Arena *const arena) noexcept :
m_arena(arena) {
    assert(arena != nullptr);
}

template<typename T>
T *ArenaAllocator<T>::allocate(size_type count) {
    if(m_arena == nullptr) {
        return nullptr;
    }

    if(count == 0) {
        count = 1;
    }

    if(count > static_cast<size_type>(std::numeric_limits<unsigned>::max())) {
        throw std::bad_alloc();
    }

    T *const data = m_arena->alloc<T>(static_cast<unsigned>(count));
    if(data == nullptr) {
        throw std::bad_alloc();
    }

    return data;
}

template<typename T>
void ArenaAllocator<T>::deallocate(T *const, const size_type) noexcept {}

template<typename T>
template <typename U>
ArenaAllocator<T>::ArenaAllocator(const ArenaAllocator<U> &arenaAllocator) noexcept :
m_arena(arenaAllocator.m_arena)
{}

template<typename T> 
CPPJSON::GeneralAllocator<T> CPPJSON::GeneralAllocator<T>::defaultAllocator;

}