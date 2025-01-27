#pragma once

#include <cstdint>
#include <string>

#define STATIC_STRING(STR) STR, sizeof(STR) - 1
#define VALID_2_BYTES_UTF16(CODEPOINT) (CODEPOINT < 0xD7FF || CODEPOINT >= 0xE000)
#define VALID_4_BYTES_UTF16(HIGH, LOW) (HIGH >= 0xD800 && HIGH <= 0xDBFF && LOW >= 0xDC00 && LOW <= 0xDFFF)

namespace CJSON {

bool         isWhitespace     (const char c);
bool         isDelimiter      (const char c);
bool         isDigit          (const char c);
uint16_t     parseCodepoint   (const char *const codepoint, bool &success);
void         utf16ToUtf8      (std::string& destination, const uint16_t codepoint);
void         utf16ToUtf8      (std::string& destination, const uint16_t high, const uint16_t low);
double       parseFloat64     (const std::string& str, bool &success);
long double  parseLongDouble  (const std::string& str, bool &success);
uint64_t     parseUint64      (const std::string& str, bool &success);
int64_t      parseInt64       (const std::string& str, bool &success);
void         printBytes       (const void *const buffer, const size_t size);
char        *file_get_contents(const std::string& path, size_t &filesize);
long         usec_timestamp   ();

}
