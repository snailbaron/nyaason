#pragma once

#include "comment_remover.hpp"

#include <string>
#include <istream>
#include <ostream>
#include <memory>
#include <utility>

enum class TokenType {
    ListStart,
    ListEnd,
    DictStart,
    DictEnd,
    KeyValueSep,
    String,
    Number,
    End,
};

struct Token {
    Token(TokenType type) : type(type) {}
    virtual void write(std::ostream& stream) const;

    TokenType type;
};

namespace token {

template <TokenType TT>
struct TokenOf : Token {
    TokenOf() : Token(TT) {}
};

struct ListStart : TokenOf<TokenType::ListStart> {};
struct ListEnd : TokenOf<TokenType::ListEnd> {};
struct DictStart : TokenOf<TokenType::DictStart> {};
struct DictEnd : TokenOf<TokenType::DictEnd> {};

struct KeyValueSep : TokenOf<TokenType::KeyValueSep> {
    explicit KeyValueSep(char symbol) : symbol(symbol) {}
    void write(std::ostream& stream) const override;

    char symbol;
};

struct String : TokenOf<TokenType::String> {
    explicit String(std::string&& string) : value(std::move(string)) {}
    void write(std::ostream& stream) const override;

    std::string value;
};

struct Number : TokenOf<TokenType::Number> {
    explicit Number(long double value) : value(value) {}
    void write(std::ostream& stream) const override;

    long double value;
};

struct End : TokenOf<TokenType::End> {};

} // namespace token

std::ostream& operator<<(std::ostream& stream, const Token& token);

class Tokenizer {
public:
    Tokenizer(std::istream& input);

    std::unique_ptr<Token> get();

private:
    struct DigitString {
        std::string string;
        unsigned long long number;
    };

    static auto eof()
    {
        return std::istream::traits_type::eof();
    }

    void next() { _current = _input.get(); }
    void skipWhitespace();

    std::string fetchStringUntil(char end);
    std::string fetchPlainString();

    CommentRemover _input;
    int _current;
};
