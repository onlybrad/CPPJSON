#pragma once

#include <array>
#include <memory>

#include "error.hpp"
#include "tokens.hpp"
#include "allocator.hpp"
#include "json.hpp"
#include "array.hpp"
#include "object.hpp"
#include "string.hpp"
#include "counters.hpp"
#include "result.hpp"

namespace CPPJSON {

typedef Result<JSON&, Error> ParserResult;

class Parser {
    struct Arenas {
        Arena object;
        Arena array;
        Arena string;
        Arena root;
    };

    struct RootNode {
        JSON      json = {};
        RootNode *next = nullptr;
    };

    typedef GeneralAllocator<Arenas> ArenasAllocator;
    typedef std::array<unsigned, 4> ArenaSizes;

    bool allocateArenas()                                               noexcept;
    bool initArenas    (const ArenaSizes arenaSizes, unsigned maxNodes) noexcept;
    bool reserveArenas (const ArenaSizes arenaCounts)                   noexcept;
    static void deallocateArenas(Arenas*)                               noexcept;

    typedef std::unique_ptr<Arenas, decltype(&deallocateArenas)> ArenasPtr;

    RootNode *newRootNode() noexcept;
    
    bool decodeStringToken(String&, Token&) noexcept;
    Error parseToken      (JSON&, Tokens&)  noexcept;
    Error parseString     (JSON&, Tokens&)  noexcept;
    Error parseArray      (JSON&, Tokens&)  noexcept;
    Error parseObject     (JSON&, Tokens&)  noexcept;
    Error parseNumber     (JSON&, Tokens&)  noexcept;
    void parseNull        (JSON&, Tokens&)  noexcept;
    void parseBool        (JSON&, Tokens&)  noexcept;

    RootNode *m_firstRoot   = nullptr;
    RootNode *m_currentRoot = nullptr;
    ArenasPtr m_arenas      = {nullptr, deallocateArenas};
    
public:
    Parser()                         noexcept;
    ~Parser()                        noexcept;
    Parser(const Parser&)                     = delete;
    Parser(Parser&&)                 noexcept = delete;
    Parser &operator=(const Parser&)          = delete;
    Parser &operator=(Parser&&)      noexcept = delete;
    
    ParserResult init     ()                             noexcept;
    ParserResult parse    (const std::string&)           noexcept;
    ParserResult parse    (const char*)                  noexcept;
    ParserResult parse    (const char*, unsigned length) noexcept;
    ParserResult parseFile(const std::string&)           noexcept;
    ParserResult parseFile(const char*)                  noexcept;

    Object::Allocator getObjectAllocator() noexcept;
    Array::Allocator  getArrayAllocator()  noexcept;
    String::Allocator getStringAllocator() noexcept;

    String createString(const std::string &);
    String createString(const char *);
};

}