// Lexer test

#include <iostream>
#include <sstream>

#include "lexer.h"

void error(char* message)
{
    printf("Error: %s\n", message);
}

int assertions = 0;
void assertEquals(int expected, int actual, char* message)
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

Lexer* buildLexer(std::stringstream &input)
{
    Lexer *lex = new Lexer(input);
    if (lex == 0)
    {
        error("unable to create lexer");
        throw ++assertions;
    }
    return lex;
}

void assertEOF(int token)
{
    assertEquals(token_eof, token, "eof expected");
}

void testEOF()
{
    std::stringstream input;
    Lexer *lex = buildLexer(input);
    assertEOF(lex->getToken());
}

void testNumber()
{
    std::stringstream input("42");
    Lexer *lex = buildLexer(input);

    int nextToken = lex->getToken();
    assertEquals(token_number, nextToken, "number expected");
    assertEOF(lex->getToken());
}

int main(int argc, char** argv)
{
    testEOF();
    testNumber();
};
