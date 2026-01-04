#include <cassert>
#include <cstring>
#include <new>

#include "memory.hpp"
#include "util.hpp"

namespace CPPJSON {

#ifndef NDEBUG

static struct AllocationStats g_allocationStats;

const AllocationStats &AllocationStats::get() noexcept {
    return g_allocationStats;
}

#endif

void *malloc(const std::size_t size) noexcept {
    void *const ret = new(std::nothrow) unsigned char[size];

#ifndef NDEBUG
    if(ret != nullptr) {
        g_allocationStats.allocated++;
    }
#endif

    return ret;
}

void *calloc(const std::size_t count, const std::size_t size) noexcept {
    if(count == 0 || Util::checkMultOverflow(count, size)) {
        return nullptr;
    }

    void *const ret = new(std::nothrow) unsigned char[count * size]();

#ifndef NDEBUG
    if(ret != nullptr) {
        g_allocationStats.allocated++;
    }
#endif

    return ret;
}

char *strdup(const char *const str) noexcept {
    assert(str != NULL);

    char *ret = static_cast<char*>(malloc(std::strlen(str) + 1));
    if(ret == nullptr) {
        return nullptr;
    }

    return std::strcpy(ret, str);
}

void free(void *ptr) noexcept {
#ifndef NDEBUG
    if(ptr == nullptr) {
        return;
    }

    g_allocationStats.deallocated++;
#endif
    delete[] static_cast<unsigned char*>(ptr);
}    

}