#include "file.hpp"

#ifdef _WIN32
    #include <windows.h>
#endif
#include <cstdio>
#include <cstdint>
#include <climits>
#include <cassert>

namespace CPPJSON {

static int fseek(std::FILE *const file, const int64_t offset, const int origin) {
#ifdef _WIN32
    return _fseeki64(file, static_cast<__int64>(offset), origin);
#elif LONG_MAX < LLONG_MAX
    return fseeko(file, static_cast<off_t>(offset), origin);
#else
    return std::fseek(file, static_cast<long>(offset), origin);
#endif
}

static std::int64_t ftell(std::FILE *const file) {
#ifdef _WIN32
    return static_cast<std::int64_t>(_ftelli64(file));
#elif LONG_MAX < LLONG_MAX
    return static_cast<std::int64_t>(ftello(file));
#else
    return static_cast<std::int64_t>(std::ftell(file));
#endif    
}

void FileContents::setData(unsigned char *const data, unsigned length) noexcept {
    m_data.reset(data);
    m_length = length;
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

FileContents FileContents::get(const char *const path) noexcept {
    assert(path != nullptr);
    assert(path[0] != '\0');

    FileContents fileContents;
#ifdef _WIN32
    const int wideLength = MultiByteToWideChar(CP_UTF8, 0, path, -1, nullptr, 0);
    if(wideLength == 0) {
        fileContents.setError(FileContents::Error::WIN32_API);
        return fileContents; 
    }

    wchar_t *wpath;
    try {
        wpath = reinterpret_cast<wchar_t*>(FileContents::Allocator::s_allocate(static_cast<std::size_t>(wideLength) * sizeof(wchar_t)));
    } catch(...) {
        fileContents.setError(FileContents::Error::MEMORY);
        return fileContents;
    }

    MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, wideLength);

    std::FILE *const file = _wfopen(wpath, L"rb");
    
    FileContents::Allocator::s_deallocate(reinterpret_cast<unsigned char*>(wpath));
#else
    std::FILE *const file = std::fopen(path, "rb");
#endif
    if(file == nullptr) {
        fileContents.setError(FileContents::Error::FOPEN);
        return fileContents;
    }
    
    std::unique_ptr<FILE, decltype(&fclose)> filePtr(file, fclose);

    if(CPPJSON::fseek(file, 0, SEEK_END) != 0) {
        fileContents.setError(FileContents::Error::FSEEK);
        return fileContents;
    }

    const std::int64_t length = CPPJSON::ftell(file);
    if(length == -1) {
        fileContents.setError(FileContents::Error::FTELL);
        return fileContents;
    }

    if(length >= static_cast<std::int64_t>(std::numeric_limits<unsigned>::max())) {
        fileContents.setError(FileContents::Error::TOO_LARGE);
        return fileContents;
    }

    if(CPPJSON::fseek(file, 0, SEEK_SET) != 0) {
        fileContents.setError(FileContents::Error::FSEEK);
        return fileContents;
    }

    unsigned char *data;
    try {
        //the buffer has 1 extra byte allocated in case a null terminated string is required
        data = FileContents::Allocator::s_allocate(static_cast<unsigned>(length) + 1U);
    } catch(...) {
        fileContents.setError(FileContents::Error::MEMORY);
        return fileContents;
    }

    if(std::fread(data, sizeof(unsigned char), static_cast<std::size_t>(length), file) !=  static_cast<std::size_t>(length)) {
        fileContents.setError(FileContents::Error::FREAD);
        FileContents::Allocator::s_deallocate(data);
        return fileContents;
    }
    data[length] = '\0';

    fileContents.setData(data, static_cast<unsigned>(length));
    
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