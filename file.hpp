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
        FSEEK,
        FTELL
    };

private:
    typedef GeneralAllocator<unsigned char> Allocator;

    typedef std::unique_ptr<unsigned char, decltype(&Allocator::s_deallocate)> Data;

    Data m_data = {nullptr, Allocator::s_deallocate};
    union {
        Error    m_error = Error::NONE;
        unsigned m_length;
    };

public:
    FileContents()                               noexcept = default;
    FileContents(const FileContents&)                     = delete;
    FileContents(FileContents&&)                 noexcept = default;
    FileContents &operator=(const FileContents&)          = delete;
    FileContents &operator=(FileContents&&)      noexcept = default;

    void setData (unsigned char *data, unsigned length) noexcept;
    void setError(Error error)                      noexcept;

    unsigned char       *getData  ()       noexcept;
    const unsigned char *getData  () const noexcept;
    Error                getError () const noexcept;
    unsigned             getLength() const noexcept;

    static FileContents get(const std::string&) noexcept;
    static FileContents get(const char*)        noexcept;
};

}