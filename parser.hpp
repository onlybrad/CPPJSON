#pragma once

#include <array>
#include <memory>

#include "tokens.hpp"
#include "allocator.hpp"
#include "json.hpp"
#include "array.hpp"
#include "object.hpp"
#include "string.hpp"
#include "counters.hpp"

namespace CPPJSON {

class Parser {
    struct Arenas {
        Arena object;
        Arena array;
        Arena string;
    };

    static void deallocateArenas(Arenas*) noexcept;
    bool        allocateArenas()          noexcept;

    typedef GeneralAllocator<Arenas> Allocator;
    typedef std::unique_ptr<Arenas, decltype(&Parser::deallocateArenas)> ArenasPtr;

    ArenasPtr m_arenas = {nullptr, Parser::deallocateArenas};
    JSON      m_json;
    Counters  m_counters;
    Tokens    m_tokens;

    bool initArenas       (const std::array<unsigned, 3> arenaSizes, unsigned maxNodes) noexcept;
    bool reserveArenas    (const std::array<unsigned, 3> arenaCounts)                   noexcept;
    bool decodeStringToken(String&, Token&) noexcept;
    bool parseToken       (JSON&, Tokens&)  noexcept;
    bool parseString      (JSON&, Tokens&)  noexcept;
    bool parseArray       (JSON&, Tokens&)  noexcept;
    bool parseObject      (JSON&, Tokens&)  noexcept;
    bool parseNumber      (JSON&, Tokens&)  noexcept;
    void parseNull        (JSON&, Tokens&)  noexcept;
    void parseBool        (JSON&, Tokens&)  noexcept;

public:
    Parser()                         noexcept;
    ~Parser()                        noexcept;
    Parser(const Parser&)                     = delete;
    Parser(Parser&&)                 noexcept = delete;
    Parser &operator=(const Parser&)          = delete;
    Parser &operator=(Parser&&)      noexcept = delete;
    
    JSON        &init     ()                             noexcept;
    JSON        &parse    (const std::string&)           noexcept;
    JSON        &parse    (const char*)                  noexcept;
    JSON        &parse    (const char*, unsigned length) noexcept;
    JSON        &parseFile(const std::string&)           noexcept;
    JSON        &parseFile(const char*)                  noexcept;
    const char  *getError ()                             noexcept;

    Object::Allocator getObjectAllocator() noexcept;
    Array::Allocator  getArrayAllocator()  noexcept;
    String::Allocator getStringAllocator() noexcept;
};

}