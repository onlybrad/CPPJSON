#pragma once

#ifndef _WIN32
    #define _FILE_OFFSET_BITS 64
    #include <unistd.h>
#endif

#include <string>

#include "allocator.hpp"

namespace CPPJSON {

class FileContents {

public:
    enum class Error {
        NONE,
        WIN32_API,
        TOO_LARGE,
        MEMORY,
        FOPEN,
        FREAD,
        FWRITE,
        FSEEK,
        FTELL,
        FCLOSE
    };

private:
    typedef GeneralAllocator<unsigned char> Allocator;

    typedef std::unique_ptr<unsigned char, decltype(&Allocator::s_deallocate)> Data;

    Data m_data = {nullptr, Allocator::s_deallocate};
    union {
        Error    m_error = Error::NONE;
        unsigned m_length;
    };

    static Error        fopen(FILE **file, const char *const path, const char *const mode) noexcept;
    static int          fseek(std::FILE*, int64_t offset, int origin)                      noexcept;
    static std::int64_t ftell(std::FILE*)                                                  noexcept;

public:
    FileContents()                               noexcept = default;
    FileContents(const FileContents&)                     = delete;
    FileContents(FileContents&&)                 noexcept = default;
    FileContents &operator=(const FileContents&)          = delete;
    FileContents &operator=(FileContents&&)      noexcept = default;

    void setData    (unsigned char *data, unsigned length = 0U) noexcept;
    void setData    (char *data,          unsigned length = 0U) noexcept;
    void setData    (std::nullptr_t)                            noexcept;
    void setError   (Error error)                               noexcept;
    
    unsigned char       *releaseData()     noexcept;
    unsigned char       *getData  ()       noexcept;
    const unsigned char *getData  () const noexcept;
    Error                getError () const noexcept;
    unsigned             getLength() const noexcept;

    Error put(const std::string &path) const noexcept;
    Error put(const char *path)        const noexcept;

    static FileContents get(const std::string& path) noexcept;
    static FileContents get(const char* path)        noexcept;
};

}