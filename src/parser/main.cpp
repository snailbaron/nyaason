#include "comment_remover.hpp"
#include "tokens.hpp"

#include <iostream>
#include <fstream>
#include <memory>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>
#include <regex>

int main()
{
    std::ifstream input("test.nyaa");
    if (!input.is_open()) {
        std::cerr << "Cannot open input file" << std::endl;
        return 1;
    }
    
    Tokenizer tokenizer(input);
    for (auto token = tokenizer.get();
            token->type != TokenType::End; token = tokenizer.get()) {
        std::cout << *token << std::endl;
    }

}