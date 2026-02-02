#include "file.hpp"

#ifdef _WIN32
    #include <windows.h>
#endif
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cassert>

namespace CPPJSON {

FileContents::Error FileContents::fopen(FILE **const file, const char *const path, const char *const mode) noexcept {
    assert(path != NULL);
    assert(mode != NULL);
    assert(std::strcmp(mode, "rb") == 0 || std::strcmp(mode, "wb") == 0);

#ifdef _WIN32
    const int wideLength = MultiByteToWideChar(CP_UTF8, 0, path, -1, nullptr, 0);
    if(wideLength == 0) {
        return FileContents::Error::WIN32_API;
    }

    wchar_t *wpath;
    try {
        wpath = reinterpret_cast<wchar_t*>(FileContents::Allocator::s_allocate(std::size_t(wideLength) * sizeof(wchar_t)));
    } catch(...) {
        return FileContents::Error::MEMORY;
    }

    MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, wideLength);

    *file = _wfopen(wpath, std::strcmp(mode, "rb") == 0 ? L"rb" : L"wb");
    
    FileContents::Allocator::s_deallocate(reinterpret_cast<unsigned char*>(wpath));
#else
    *file = std::fopen(path, mode);
#endif
    return *file == nullptr
        ? FileContents::Error::FOPEN
        : FileContents::Error::NONE;
}

int FileContents::fseek(std::FILE *const file, const std::int64_t offset, const int origin) noexcept {
#ifdef _WIN32
    return _fseeki64(file, (__int64)(offset), origin);
#elif LONG_MAX < LLONG_MAX
    return fseeko(file, (off_t)(offset), origin);
#else
    return std::fseek(file, long(offset), origin);
#endif
}

std::int64_t FileContents::ftell(std::FILE *const file) noexcept {
#ifdef _WIN32
    return std::int64_t(_ftelli64(file));
#elif LONG_MAX < LLONG_MAX
    return std::int64_t(ftello(file));
#else
    return std::int64_t(std::ftell(file));
#endif    
}

void FileContents::setData(unsigned char *const data, const unsigned length) noexcept {
    assert(data != nullptr || length == 0);

    m_data.reset(data);
    m_length = length;
}

void FileContents::setData(char *const data, const unsigned length) noexcept {
    assert(data != nullptr || length == 0);

    setData(reinterpret_cast<unsigned char*>(data), length);
}

void FileContents::setData(const std::nullptr_t) noexcept {
    m_data.reset(nullptr);
    m_length = 0U;
}

unsigned char *FileContents::releaseData() noexcept {
   unsigned char *const data = m_data.release();
   m_length = 0U;
   return data;
}

unsigned char *FileContents::getData() noexcept {
    return m_data.get();
}

const unsigned char *FileContents::getData() const noexcept {
    return m_data.get();
}

FileContents::Error FileContents::getError() const noexcept {
    return m_data == nullptr ? m_error : Error::NONE;
}

unsigned FileContents::getLength() const noexcept {
    return m_data == nullptr ? 0U : m_length;
}

FileContents::Error FileContents::put(const std::string &path) const noexcept {
    assert(path[0] != '\0');

    return put(path.c_str());
}

FileContents::Error FileContents::put(const char *const path) const noexcept {
    assert(path != nullptr);
    assert(path[0] != '\0');

    if(getLength() == 0) {
        return FileContents::Error::NONE;
    }

    FILE *file;
    const FileContents::Error error = FileContents::fopen(&file, path, "wb");
    if(error != FileContents::Error::NONE) {
        return error;
    }

    if(std::fwrite(
        m_data.get(),
        sizeof(m_data.get()[0]),
        std::size_t(m_length),
        file
    ) != std::size_t(m_length)) {
        std::fclose(file);
        return FileContents::Error::FWRITE;
    }

    return std::fclose(file) != 0
        ? FileContents::Error::FCLOSE
        : FileContents::Error::NONE;
}

FileContents FileContents::get(const char *const path) noexcept {
    assert(path != nullptr);
    assert(path[0] != '\0');

    FileContents fileContents;

    FILE *file;
    const FileContents::Error error = FileContents::fopen(&file, path, "rb");
    if(error != FileContents::Error::NONE) {
        fileContents.setError(error);
        return fileContents;
    }

    std::unique_ptr<FILE, decltype(&std::fclose)> filePtr(file, std::fclose);

    if(FileContents::fseek(file, 0, SEEK_END) != 0) {
        fileContents.setError(FileContents::Error::FSEEK);
        return fileContents;
    }

    const std::int64_t length = FileContents::ftell(file);
    if(length == -1) {
        fileContents.setError(FileContents::Error::FTELL);
        return fileContents;
    }

    if(length >= std::int64_t(std::numeric_limits<unsigned>::max())) {
        fileContents.setError(FileContents::Error::TOO_LARGE);
        return fileContents;
    }

    if(FileContents::fseek(file, 0, SEEK_SET) != 0) {
        fileContents.setError(FileContents::Error::FSEEK);
        return fileContents;
    }

    unsigned char *data;
    try {
        //the buffer has 1 extra byte allocated in case a null terminated string is required
        data = FileContents::Allocator::s_allocate(unsigned(length) + 1U);
    } catch(...) {
        fileContents.setError(FileContents::Error::MEMORY);
        return fileContents;
    }

    if(std::fread(
        data,
        sizeof(data[0]),
        std::size_t(length),
        file
    ) !=  std::size_t(length)) {
        fileContents.setError(FileContents::Error::FREAD);
        FileContents::Allocator::s_deallocate(data);
        return fileContents;
    }
    data[length] = '\0';

    fileContents.setData(data, unsigned(length));
    
    return fileContents;
}

FileContents FileContents::get(const std::string &path) noexcept {
    assert(path[0] != '\0');

    return get(path.c_str());
}

void FileContents::setError(const Error error) noexcept {
    m_data.reset();
    m_error = error;
}


}