#include "parser.hpp"
#include <stdexcept>
#include <sstream>

namespace {

void reportUnexpectedToken(const std::unique_ptr<Token>& token)
{
    std::ostringstream error;
    error << "Unexpected token: " << *token;
    throw std::runtime_error(error.str());
}

} // namespace

nyaa::Nyaason Parser::parse()
{
    auto token = _tokens->get();
    switch (token->type) {
        case TokenType::DictStart: return parseDictionary();
        case TokenType::KeyValueSep: return parseStructure();
        case TokenType::ListStart: return parseList();
        case TokenType::Number: return parseNumber();
        case TokenType::String:
        {
            // TODO: do something about casts
            token::String* string = static_cast<token::String*>(token.get());

            auto secondToken = _tokens->get();
            if (secondToken->type == TokenType::End) {
                return nyaa::Nyaason(new nyaa::String(string->value));
            } else if (secondToken->type == TokenType::KeyValueSep) {
                // TODO: read key-value pair list, considering it a dictionary
            } else {
                reportUnexpectedToken(secondToken);
            }
        }
        case TokenType::End: return nyaa::Nyaason();
    };

    reportUnexpectedToken(token);
}

nyaa::Nyaason Parser::parseDictionary()
{
    auto dictionary = std::make_unique<nyaa::Dictionary>();
    for (auto token = _tokens->get();
            token->type != TokenType::DictEnd; token = _tokens->get()) {

    }
}

nyaa::Nyaason Parser::parseStructure()
{
}

nyaa::Nyaason Parser::parseList()
{
}

nyaa::Nyaason Parser::parseNumber()
{
}
