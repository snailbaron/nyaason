#pragma once

#include <istream>

class CommentRemover {
public:
    CommentRemover(std::istream& input);

    int get();

private:
    static auto eof()
    {
        return std::istream::traits_type::eof();
    }

    /**
    * Skip current line of input (that is, read and skip characters until a
    * line break or an end of file). Return first character of the next line,
    * or eof() if this is the final line of input.
    */
    int getAtNextLine();

    std::istream& _input;
};
