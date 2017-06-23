#pragma once

#include "comment_remover.hpp"

#include <string>
#include <istream>
#include <ostream>
#include <memory>
#include <utility>
#include <typeinfo>

class Token {
public:
    enum class Type {
        ListStart,
        ListEnd,
        DictStart,
        DictEnd,
        KeyValueSep,
        String,
        End,
    };

    Token(Type type, std::string string = "")
        : _type(type), _string(std::move(string)) {}

    const Type& type() const { return _type; }
    friend std::ostream& operator<<(std::ostream& stream, const Token& token);

private:
    Type _type;
    std::string _string;
};

class Tokenizer {
public:
    Tokenizer(std::istream& input);

    Token get();

private:
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
