#pragma once

#include <cstdint>

namespace CPPJSON {

#ifndef NDEBUG

struct AllocationStats {
    unsigned allocated   = 0U;
    unsigned deallocated = 0U;

    static const AllocationStats &get() noexcept;
};

#endif

void *malloc(std::size_t)                         noexcept;
void *calloc(std::size_t count, std::size_t size) noexcept;
char *strdup(const char*)                         noexcept;
void  free  (void*)                               noexcept;

}