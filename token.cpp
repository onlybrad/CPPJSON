#include "token.hpp"

namespace CJSON {

Token::Token() : 
    type(TokenType::INVALID), 
    value(nullptr) 
    {}

Token::~Token() {
    type = TokenType::INVALID;
    value.data = nullptr;
    value.size = 0U;
}

Token::Token(Token &&other) : 
    type(other.type), 
    value(other.value.data, other.value.size) {
    if(this != &other) {
        other.type = TokenType::INVALID;
        other.value.data = nullptr;
        other.value.size = 0U;
    }
}

Tokens::Tokens() {
    data.reserve(16);
}

Token *Tokens::nextToken() {
    data.emplace_back();

    return &data.back();
}

}