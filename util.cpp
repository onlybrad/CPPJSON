#ifdef _WIN32
#include <windows.h>
#include <memory>
#endif
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <cstdio>
#include <sys/time.h>
#include "util.hpp"

namespace CJSON {

bool isWhitespace(const char c) {
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

bool isDelimiter(const char c) {
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

bool isDigit(const char c) {
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

uint16_t parseCodepoint(const char *const codepoint, bool &success) {
    char hex[7] = "0x";
    std::memcpy(hex + 2, codepoint, 4);
    char *end_ptr;
    errno = 0;

    const uint16_t ret = (uint16_t)std::strtoul(hex, &end_ptr, 16);
    if(end_ptr == hex || *end_ptr != '\0' || errno == ERANGE) {
        success = false;
        return (uint16_t)0;
    }

    success = true;
    return ret;
}

void utf16ToUtf8(std::string& destination, const uint16_t codepoint) {
    if(codepoint <= 0x7F) {
        destination.push_back((char)(codepoint & 0x7f));
    } else if(codepoint <= 0x7FF) {
        destination.push_back((char)((codepoint  >> 6)         | 0xC0));
        destination.push_back((char)(((codepoint >> 0) & 0x3F) | 0x80));
    } else {
        destination.push_back((char)((codepoint  >> 12)        | 0xE0));
        destination.push_back((char)(((codepoint >> 6) & 0x3F) | 0x80));
        destination.push_back((char)(((codepoint >> 0) & 0x3F) | 0x80));
    }
}

void utf16ToUtf8(std::string& destination, const uint16_t high, const uint16_t low) {
    const uint32_t codepoint = (uint32_t)(((high - 0xD800) << 10) | (low - 0xDC00)) + 0x10000;

    destination.push_back((char)((codepoint  >> 18)         | 0xF0));
    destination.push_back((char)(((codepoint >> 12) & 0x3F) | 0x80));
    destination.push_back((char)(((codepoint >> 6)  & 0x3F) | 0x80));
    destination.push_back((char)(((codepoint >> 0)  & 0x3F) | 0x80));
}

double parseFloat64(const std::string& str, bool &success) {
    char *end_ptr;
    errno = 0;
    
    const double ret = std::strtod(str.c_str(), &end_ptr);
    if(end_ptr == str || *end_ptr != '\0' || errno == ERANGE) {
        success = false;
        return 0.0;
    }

    success = true;
    return ret;
}

long double parseLongDouble(const std::string& str, bool &success) {
    char *end_ptr;
    errno = 0;
    
    const long double ret = std::strtold(str.c_str(), &end_ptr);
    if(end_ptr == str || *end_ptr != '\0' || errno == ERANGE) {
        success = false;
        return 0.0L;
    }

    success = true;
    return ret;
}

uint64_t parseUint64(const std::string& str, bool &success) {
    char *end_ptr;
    errno = 0;

    const uint64_t ret = std::strtoull(str.c_str(), &end_ptr, 10);
    if(end_ptr == str || *end_ptr != '\0' || errno == ERANGE) {
        success = false;
        return (uint64_t)0;
    }

    success = true;
    return ret;
}

int64_t parseInt64(const std::string& str, bool &success) {
    char *end_ptr;
    errno = 0;
    
    const int64_t ret = strtoll(str.c_str(), &end_ptr, 10);
    if(end_ptr == str || *end_ptr != '\0' || errno == ERANGE) {
        success = false;
        return (int64_t)0;
    }

    success = true;
    return ret;
}

void printBytes(const void *const buffer, const size_t size) {
    std::putchar('[');
    for(size_t i = 0; i < size - 1; i++) {
        std::printf("0x%02hhx, ", ((const unsigned char*)buffer)[i]);
    }
    std::printf("0x%02hhx]\n", ((const unsigned char*)buffer)[size - 1]);
}

char *file_get_contents(const std::string& path, size_t &filesize) {
#ifdef _WIN32
    const int wide_length = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, NULL, 0);
    if(wide_length == 0) {
        filesize = 0;
        return NULL;
    }

    std::unique_ptr<wchar_t[]> wpath(new wchar_t[wide_length]);

    MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, wpath.get(), wide_length);
    FILE *const file = _wfopen(wpath.get(), L"rb");
#else
    FILE *const file = fopen(path, "rb");
#endif
    if(file == NULL) {
        filesize = 0;
        return NULL;
    }

    fseeko(file, 0, SEEK_END);
    const off_t length = ftello(file);
    fseeko(file, 0, SEEK_SET);
    //the buffer returned has 1 extra byte allocated in case a null terminated string is required
    char *data = new char[length + 1];
    fread(data, sizeof(char), (size_t)length, file);
    fclose(file);

    filesize = (size_t)length;
    return data;
}

long usec_timestamp() {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    return current_time.tv_sec * 1000000L + current_time.tv_usec;
}

}
