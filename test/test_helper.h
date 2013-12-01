// Lexer test

#include <iostream>
#include <sstream>

#include "lexer.h"

void error(const char* message)
{
    printf("Error: %s\n", message);
}

int assertions = 0;
void assertEquals(int expected, int actual, const char* message)
{
    ++assertions;
    if (expected != actual)
    {
        printf(
            "assertion failed - expected %i, actual %i. %s.\n",
            expected,
            actual,
            message
        );
        throw assertions;
    }
}

Lexer *buildLexer(std::stringstream &input)
{
    Lexer *lex = new Lexer(input);
    if (lex == 0)
    {
        error("unable to create lexer");
        throw ++assertions;
    }
    return lex;
}
