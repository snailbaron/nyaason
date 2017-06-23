#include "comment_remover.hpp"
#include "tokens.hpp"
#include "parser.hpp"

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

    auto tokenizer = std::make_shared<Tokenizer>(input);
    Parser parser(tokenizer);



}