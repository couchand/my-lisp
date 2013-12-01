// Lexer test

#include <iostream>
#include <sstream>

#include "test_helper.h"

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

void testInteger()
{
    std::stringstream input("42");
    Lexer *lex = buildLexer(input);

    int nextToken = lex->getToken();
    assertEquals(token_number, nextToken, "number expected");
    assertEquals(42, lex->getLastNumber(), "keep the last number");
    assertEOF(lex->getToken());
}

void testDecimal()
{
    std::stringstream input("3.141592653589793238");
    Lexer *lex = buildLexer(input);

    int nextToken = lex->getToken();
    assertEquals(token_number, nextToken, "number expected");
    assertEquals(3.141592653589793238, lex->getLastNumber(), "keep the last number");
    assertEOF(lex->getToken());
}

void testIdentifier()
{
    std::stringstream input("foobar");
    Lexer *lex = buildLexer(input);

    int nextToken = lex->getToken();
    assertEquals(token_identifier, nextToken, "identifier expected");
    assertEOF(lex->getToken());
}

void testMultiple()
{
    std::stringstream input("i am a very model of 4 1337 general )");
    Lexer *lex = buildLexer(input);

    assertEquals(token_identifier, lex->getToken(), "i");
    assertEquals(token_identifier, lex->getToken(), "am");
    assertEquals(token_identifier, lex->getToken(), "a");
    assertEquals(token_identifier, lex->getToken(), "very");
    assertEquals(token_identifier, lex->getToken(), "model");
    assertEquals(token_identifier, lex->getToken(), "of");
    assertEquals(token_number, lex->getToken(), "4");
    assertEquals(token_number, lex->getToken(), "1337");
    assertEquals(token_identifier, lex->getToken(), "general");
    assertEquals(')', lex->getToken(), ")");
}

void testLexer()
{
    testEOF();
    testInteger();
    testDecimal();
    testIdentifier();
    testMultiple();
}
