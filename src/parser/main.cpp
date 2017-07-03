#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "tokens.hpp"

int main()
{
    std::ifstream ifs("test.nyaa");
    Tokenizer tokenizer(ifs);

    for (Token token = tokenizer.get(); token.type != Token::Type::End;
            token = tokenizer.get()) {
        std::cout << token << std::endl;
    }

}