#include <cassert>

#include "tokens.hpp"

namespace CPPJSON {

const unsigned Tokens::MINIMUM_CAPACITY = 8U;

bool Tokens::reserve(unsigned capacity) noexcept {
    if(capacity < MINIMUM_CAPACITY) {
        capacity = MINIMUM_CAPACITY;
    }

    try {
        data.reserve(std::size_t(capacity));
        return true;
    } catch(...) {
        return false;
    }
}

void Tokens::reset() noexcept {
    data.clear();
    currentToken = data.data();
}

CPPJSON::Token *Tokens::nextToken() noexcept {
    try {
        data.emplace_back();
        return &data.back();
    } catch(...) {
        return nullptr;
    }
}

}