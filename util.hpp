#pragma once

#include <cstdint>
#include <memory>
#include <limits>
#include "result.hpp"

#ifndef static_strlen
    #define static_strlen(STR) (sizeof(STR) - 1)
#endif

namespace CPPJSON {
namespace Util {

bool          isWhitespace   (char c)                                         noexcept;
bool          isDelimiter    (char c)                                         noexcept;
bool          isControlChar  (char c)                                         noexcept;
std::uint16_t hexToUtf16     (const char *unicode, bool &success)             noexcept;
char*         utf16ToUtf8    (char *destination, uint16_t codepoint)          noexcept;
char*         utf16ToUtf8    (char *destination, uint16_t high, uint16_t low) noexcept;
bool          isValidUtf16   (std::uint16_t high)                             noexcept;
bool          isValidUtf16   (std::uint16_t high, std::uint16_t low)          noexcept; 
double        parseFloat64   (const char *str, bool &success)                 noexcept;
long double   parseLongDouble(const char *str, bool &success)                 noexcept;
std::uint64_t parseUint64    (const char *str, bool &success)                 noexcept;
std::int64_t  parseInt64     (const char *str, bool &success)                 noexcept;
void          printBytes     (const void *buffer, const size_t size)          noexcept;
std::uint64_t usecTimestamp  ()                                               noexcept;

template<
    typename T,
    typename std::enable_if<std::is_unsigned<T>::value, bool>::type = true
>
Result<T> safeMult(T a, T b) noexcept {
    if(b == 0) {
        Result<T>::fromValue(0U);
    }

    if(a > std::numeric_limits<T>::max() / b) {
        return Result<T>::fromError(true);
    }

    return Result<T>::fromValue(a * b);
}

template<
    typename T,
    typename std::enable_if<std::is_unsigned<T>::value, bool>::type = true
>
bool checkMultOverflow(T a, T b) noexcept {
    return b != 0 && a > std::numeric_limits<T>::max() / b;
}

}
}