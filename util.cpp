#if defined(__MINGW32__) || !defined(_WIN32)
    #include <sys/time.h>
#endif
#ifdef _WIN32
    #include <windows.h>
#endif

#include <cassert>
#include <cstdio>
#include <cerrno>
#include <cstring>

#include "util.hpp"
#include "allocator.hpp"

namespace CPPJSON {

namespace Util {

bool isWhitespace(const char c) noexcept {
    switch(c) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
        return true;
    default:
        return false;
    }
}

bool isDelimiter(const char c) noexcept {
    switch(c) {
    case '[':
    case ']':
    case '}':
    case '{':
    case ',':
    case ':':
        return true;
    default:
        return false;
    }
}

bool isDigit(const char c) noexcept {
    switch(c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return true;
    default:
        return false;
    }
}

std::uint16_t hexToUtf16(const char *const codepoint, bool &success) noexcept {
    assert(codepoint != nullptr);

    char hex[7] = "0x";
    std::memcpy(hex + 2, codepoint, 4U);
    char *endPtr;
    errno = 0;

    const std::uint16_t ret = static_cast<std::uint16_t>(std::strtoul(hex, &endPtr, 16));
    if(endPtr == hex || *endPtr != '\0' || errno == ERANGE) {
        success = false;
        return static_cast<std::uint16_t>(0U);
    }

    success = true;
    return ret;
}

char *utf16ToUtf8(char *const destination, const std::uint16_t high) noexcept {
    assert(destination != nullptr);

    if(high <= 0x7F) {
        destination[0] = static_cast<char>((high & 0x7f));
        destination[1] = '\0';
    } else if(high <= 0x7FF) {
        destination[0] = static_cast<char>(((high  >> 6)         | 0xC0));
        destination[1] = static_cast<char>((((high >> 0) & 0x3F) | 0x80));
        destination[2] = '\0';
    } else {
        destination[0] = static_cast<char>(((high  >> 12)        | 0xE0));
        destination[1] = static_cast<char>((((high >> 6) & 0x3F) | 0x80));
        destination[2] = static_cast<char>((((high >> 0) & 0x3F) | 0x80));
        destination[3] = '\0';
    }

    return destination;
}

bool isValidUtf16(const std::uint16_t high) noexcept {
    return high < 0xD7FF || high >= 0xE000;
}

bool isValidUtf16(const std::uint16_t high, const std::uint16_t low) noexcept {
    return high >= 0xD800 && high <= 0xDBFF && low >= 0xDC00 && low <= 0xDFFF;
}

char *utf16ToUtf8(char *const destination, const std::uint16_t high, const std::uint16_t low) noexcept {
    assert(destination != nullptr);

    const std::uint32_t codepoint = static_cast<std::uint32_t>((((high - 0xD800) << 10) | (low - 0xDC00)) + 0x10000);

    destination[0] = static_cast<char>(((codepoint  >> 18)         | 0xF0));
    destination[1] = static_cast<char>((((codepoint >> 12) & 0x3F) | 0x80));
    destination[2] = static_cast<char>((((codepoint >> 6)  & 0x3F) | 0x80));
    destination[3] = static_cast<char>((((codepoint >> 0)  & 0x3F) | 0x80));
    destination[4] = '\0';

    return destination;
}

double parseFloat64(const char *const str, bool &success) noexcept {
    assert(str != nullptr);
    
    char *endPtr;
    errno = 0;
    
    const double ret = std::strtod(str, &endPtr);
    if(endPtr == str || *endPtr != '\0' || errno == ERANGE) {
        success = false;
        return 0.0;
    }

    success = true;
    return ret;
}

long double parseLongDouble(const char *const str, bool &success) noexcept {
    assert(str != nullptr);
    
    char *endPtr;
    errno = 0;
    
    const long double ret = std::strtold(str, &endPtr);
    if(endPtr == str || *endPtr != '\0' || errno == ERANGE) {
        success = false;
        return 0.0L;
    }

    success = true;
    return ret;
}

std::uint64_t parseUint64(const char *const str, bool &success) noexcept {
    assert(str != nullptr);
    
    char *endPtr;
    errno = 0;

    const uint64_t ret = std::strtoull(str, &endPtr, 10);
    if(endPtr == str || *endPtr != '\0' || errno == ERANGE) {
        success = false;
        return 0;
    }

    success = true;
    return ret;
}

std::int64_t parseInt64(const char *const str, bool &success) noexcept {
    assert(str != nullptr);
    
    char *endPtr;
    errno = 0;
    
    const std::int64_t ret = std::strtoll(str, &endPtr, 10);
    if(endPtr == str || *endPtr != '\0' || errno == ERANGE) {
        success = false;
        return 0;
    }

    success = true;
    return ret;
}

void printBytes(const void *const buffer, const size_t size) noexcept {
    assert(buffer != nullptr);
    assert(size > 0);

    const unsigned char *bytes = static_cast<const unsigned char*>(buffer);

    std::putchar('[');
    for(std::size_t i = 0; i < size - 1; i++) {
        std::printf("0x%02hhx, ", bytes[i]);
    }
    std::printf("0x%02hhx]\n", bytes[size - 1]);
}

std::uint64_t usecTimestamp() noexcept {
#if defined(__MINGW32__) || !defined(_WIN32)
    struct timeval current_time;
    gettimeofday(&current_time, nullptr);
    return static_cast<std::uint64_t>(current_time.tv_sec * 1000000L + current_time.tv_usec);
#elif defined(_WIN32)
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    std::uint64_t tt = static_cast<std::uint64_t>(ft.dwHighDateTime);
    tt <<= 32ULL;
    tt |= static_cast<std::uint64_t>(ft.dwLowDateTime);
    tt /= 10ULL;
    tt -= 11644473600000000ULL;
    return tt;
#else
    #error "Unknown platform. Missing implementation for usec_timestamp."
#endif
}

}

}