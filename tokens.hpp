#pragma once

#include <memory>
#include <vector>

#include "token.hpp"
#include "allocator.hpp"

namespace CPPJSON {

class Parser;
class Lexer;

struct Tokens {
    static constexpr unsigned MINIMUM_CAPACITY = 8U;

    typedef GeneralAllocator<Token>       Allocator;
    typedef std::vector<Token, Allocator> Container;

    Container data{0, Allocator()};
    Token    *currentToken = nullptr;  

    Tokens()                         noexcept = default;
    Tokens(const Tokens&)                     = delete;
    Tokens(Tokens&&)                 noexcept = default;
    Tokens& operator=(const Tokens&)          = delete;
    Tokens& operator=(Tokens&&)      noexcept = default;

    bool   reserve     (unsigned capacity = 0U) noexcept;
    void   reset       ()                       noexcept;
    Token *nextToken()                          noexcept;
};

}