#include "tokens.hpp"

#include <cctype>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>
#include <set>

std::ostream& operator<<(std::ostream& stream, const Token& token)
{
    static const std::map<Token::Type, std::string> typeNames{
        { Token::Type::ListStart,   "ListStart   -- " },
        { Token::Type::ListEnd,     "ListEnd     -- " },
        { Token::Type::DictStart,   "DictStart   -- " },
        { Token::Type::DictEnd,     "DictEnd     -- " },
        { Token::Type::KeyValueSep, "KeyValueSep -- " },
        { Token::Type::String,      "String      -- " },
        { Token::Type::End,         "End         -- " },
    };

    auto it = typeNames.find(token._type);
    if (it == typeNames.end()) {
        throw std::runtime_error("tokenTypeName: unknown token type");
    }

    stream << it->second << token._string;
    return stream;
}

Tokenizer::Tokenizer(std::istream& input)
    : _input(input)
{
    next();
}

void Tokenizer::skipWhitespace()
{
    while (std::isspace(_current)) {
        next();
    }
}

std::string Tokenizer::fetchStringUntil(const char end)
{
    std::ostringstream value;
    next();
    for (; _current != end; next()) {
        if (_current == '\\') {
            next();
            if (_current == '\\') value << '\\';
            else if (_current == end) value << static_cast<char>(end);
            else if (_current == 'b') value << '\b';
            else if (_current == 'f') value << '\f';
            else if (_current == 'n') value << '\n';
            else if (_current == 'r') value << '\r';
            else if (_current == 't') value << '\t';
            else {
                throw std::runtime_error("Unknown escape sequence in string");
            }
        } else {
            value << static_cast<char>(_current);
        }
    }
    next();
    return value.str();
}

std::string Tokenizer::fetchPlainString()
{
    auto isPlainStringChar = [this](int c) {
        std::set<char> forbiddenChars {
            '\'', '"', '[', ']', '{', '}', ':', '=',
        };
        return c != eof() && !std::isspace(c) &&
            forbiddenChars.find(static_cast<char>(c)) == forbiddenChars.end();
    };

    std::ostringstream stream;
    while (isPlainStringChar(_current)) {
        stream << static_cast<char>(_current);
        next();
    }
    return stream.str();
}

Token Tokenizer::get()
{
    using Type = Token::Type;

    skipWhitespace();
    if (_current == eof()) {
        return Token(Type::End);
    }

    auto give = [this](Type type, std::string string = "") {
        next();
        return Token(type, std::move(string));
    };


    switch (_current) {
        // Single-character tokens
        case '[': return give(Type::ListStart);
        case ']': return give(Type::ListEnd);
        case '{': return give(Type::DictStart);
        case '}': return give(Type::DictEnd);

        case ':':
        case '=':
        {
            std::ostringstream stream;
            stream << static_cast<char>(_current);
            return give(Type::KeyValueSep, stream.str());
        }

        // Quoted strings
        case '\'':
        case '"':
        {
            std::string value = fetchStringUntil(_current);
            return give(Type::String, std::move(value));
        }
    }

    // Non-quoted text. It is either a number, or a plain string.
    std::string plainString = fetchPlainString();
    return Token(Type::String, std::move(plainString));
}
