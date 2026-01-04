#pragma once

#include <string>

#include "token.hpp"
#include "tokens.hpp"
#include "allocator.hpp"
#include "counters.hpp"

namespace CPPJSON {

class Lexer {   
    const char *const m_data     = nullptr;
    const unsigned    m_length   = 0U;
    unsigned          m_position = 0U;

    void skipWhitespace          ()                               noexcept;
    bool readString              (Token&)                         noexcept;
    bool readNumber              (Token&)                         noexcept;
    bool readKeyword             (Token&)                         noexcept;
    void readInvalidToken        (Token&)                         noexcept;
    bool countCountainersElements(Tokens &tokens, Counters &counters) noexcept;

public:
    enum class Error {
        NONE,
        TOKEN,
        MEMORY,
    };
    
    Lexer(const char *data, unsigned length) noexcept;
    Lexer()                                  noexcept = delete;

    Error tokenize(Tokens &tokens, Counters &counters) noexcept;
};

}