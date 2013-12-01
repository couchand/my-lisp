// Lexer test

#include <iostream>
#include <sstream>
#include <cstring>

#include "test_helper.h"

namespace
{

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
    if (3.141592653589793238 != lex->getLastNumber()) throw "keep the last number";
    assertEOF(lex->getToken());
}

void testIdentifier()
{
    std::stringstream input("foobar");
    Lexer *lex = buildLexer(input);

    int nextToken = lex->getToken();
    assertEquals(token_identifier, nextToken, "identifier expected");
    if (strncmp("foobar", lex->getLastIdentifier(), 6))
        throw "expected foobar";
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
    assertEOF(lex->getToken());
}

void testWhitespace()
{
    std::stringstream input("  1     2 \t  3 \n 4 \r 5 \n\r6\n7\n\t\t8\n\n\n  ");
    Lexer *lex = buildLexer(input);

    assertEquals(token_number, lex->getToken(), "1");
    assertEquals(token_number, lex->getToken(), "2");
    assertEquals(token_number, lex->getToken(), "3");
    assertEquals(token_number, lex->getToken(), "4");
    assertEquals(token_number, lex->getToken(), "5");
    assertEquals(token_number, lex->getToken(), "6");
    assertEquals(token_number, lex->getToken(), "7");
    assertEquals(token_number, lex->getToken(), "8");
    assertEOF(lex->getToken());
}
}

void testLexer()
{
    testEOF();
    testInteger();
    testDecimal();
    testIdentifier();
    testMultiple();
    testWhitespace();
}
