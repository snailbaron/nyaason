#include "tokens.hpp"

#include <cctype>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>
#include <set>

using namespace token;

namespace {

const std::map<TokenType, std::string> tokenTypeNames {
    { TokenType::ListStart,   "ListStart   -- [" },
    { TokenType::ListEnd,     "ListEnd     -- ]" },
    { TokenType::DictStart,   "DictStart   -- {" },
    { TokenType::DictEnd,     "DictEnd     -- }" },
    { TokenType::KeyValueSep, "KeyValueSep -- "  },
    { TokenType::String,      "String      -- "  },
    { TokenType::Number,      "Number      -- "  },
    { TokenType::End,         "End         -- "  },
};

} // namespace

void Token::write(std::ostream& stream) const
{
    stream << tokenTypeNames.at(type);
}

std::ostream& operator<<(std::ostream& stream, const Token& token)
{
    token.write(stream);
    return stream;
}

void KeyValueSep::write(std::ostream& stream) const
{
    stream << tokenTypeNames.at(type) << symbol;
}

void String::write(std::ostream& stream) const
{
    stream << tokenTypeNames.at(type) << value;
}

void Number::write(std::ostream& stream) const
{
    stream << tokenTypeNames.at(type) << value;
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

std::unique_ptr<Token> Tokenizer::get()
{
    skipWhitespace();
    if (_current == eof()) {
        return std::make_unique<token::End>();
    }

    auto give = [this](std::unique_ptr<Token> token) {
        next();
        return token;
    };

    switch (_current) {
        // Single-character tokens
        case '[': return give(std::make_unique<token::ListStart>());
        case ']': return give(std::make_unique<token::ListEnd>());
        case '{': return give(std::make_unique<token::DictStart>());
        case '}': return give(std::make_unique<token::DictEnd>());

        case ':':
        case '=':
            return give(std::make_unique<token::KeyValueSep>(_current));

        // Quoted strings
        case '\'':
        case '"':
        {
            std::string value = fetchStringUntil(_current);
            return std::make_unique<token::String>(std::move(value));
        }
    }

    // Non-quoted text. It is either a number, or a plain string.
    std::string plainString = fetchPlainString();
    try {
        long double number = std::stold(plainString);
        return std::make_unique<token::Number>(number);
    } catch (std::invalid_argument&) {
        return std::make_unique<token::String>(std::move(plainString));
    } catch (std::out_of_range&) {
        std::cerr <<
            "Numeric value is out of range: " << plainString << std::endl;
        throw;
    }
}
