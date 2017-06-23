#pragma once

#include "tokens.hpp"
#include "objects.hpp"

#include <memory>
#include <type_traits>

class Parser {
public:
    Parser(std::shared_ptr<Tokenizer> tokenizer)
        : _tokens(tokenizer) {}

    nyaa::Nyaason parse();

private:
    nyaa::Nyaason parseDictionary();
    nyaa::Nyaason parseStructure();
    nyaa::Nyaason parseList();
    nyaa::Nyaason parseNumber();

    template <class Tok>
    Tok* tokenCast(const std::unique_ptr<Token>& token)
    {
        static_assert(std::is_base_of<Token, Tok>(),
            "tokenCast: Token is not a base of requested type");

    }

    std::shared_ptr<Tokenizer> _tokens;
};