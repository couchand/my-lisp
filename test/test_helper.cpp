#include "test_helper.h"

void error(const char* message)
{
    printf("Error: %s\n", message);
}

void assertEquals(int expected, int actual, const char* message)
{
    if (expected != actual)
    {
        printf(
            "assertion failed - expected %i, actual %i. %s.\n",
            expected,
            actual,
            message
        );
        throw "test failed";
    }
}

Lexer *buildLexer(std::stringstream &input)
{
    Lexer *lex = new Lexer(input);
    if (lex == 0)
    {
        error("unable to create lexer");
        throw "test error";
    }
    return lex;
}

Parser *buildParser(Lexer *lexer)
{
    Parser *parser = new Parser(lexer);
    if (parser == 0)
    {
        error("unable to create parser");
        throw "test error";
    }
    return parser;
}
