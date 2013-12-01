// Lexer test

#include <cstdio>

#include "lexer.h"

void error(char* message)
{
    printf("Error: %s\n", message);
}

int assertEquals(int expected, int actual, char* message)
{
    if (expected != actual)
    {
        printf(
            "assertion failed - expected %i, actual %i. %s.\n",
            expected,
            actual,
            message
        );
    }
    return 0;
}

int main(int argc, char** argv)
{
    return
        testEOF();
};

int testEOF()
{
    Lexer *lex = new Lexer();
    if (lex == 0)
    {
        error("unable to create lexer");
        return 1;
    }

    int nextToken = lex->getToken();

    int ret = assertEquals(token_eof, nextToken, "eof expected");

    return ret;
}
