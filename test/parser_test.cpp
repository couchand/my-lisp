// Parser test

#include <iostream>
#include <sstream>
#include <cstring>

#include "test_helper.h"

namespace
{
void testInteger()
{
    std::stringstream input("42");
    Parser *par = buildParser(input);

    AST::Expression *parsed = par->parse();
    if (parsed == 0)
    {
        error("unable to parse expression");
    }

    AST::NumberLiteral *num = dynamic_cast<AST::NumberLiteral*>(parsed);
    if (num == 0)
    {
        error("unexpected expression type");
    }

    if (num->getValue() != 42)
    {
        error("expected 42");
    }
}

void testDecimal()
{
    std::stringstream input("3.141592653589793238");
    Parser *par = buildParser(input);

    AST::Expression *parsed = par->parse();
    if (parsed == 0)
    {
        error("unable to parse expression");
    }

    AST::NumberLiteral *num = dynamic_cast<AST::NumberLiteral*>(parsed);
    if (num == 0)
    {
        error("unexpected expression type");
    }

    if (num->getValue() != 3.141592653589793238)
    {
        error("expected pi");
    }
}

void testIdentifier()
{
    std::stringstream input("foobar");
    Parser *par = buildParser(input);

    AST::Expression *parsed = par->parse();
    if (parsed == 0)
    {
        error("unable to parse expression");
    }

    AST::Identifier *id = dynamic_cast<AST::Identifier*>(parsed);
    if (id == 0)
    {
        error("unexpected expression type");
    }

    const char* name = id->getName();
    if (strncmp(name, "foobar", 5))
    {
        error("expected foobar");
    }
}

void testCall()
{
    std::stringstream input("cons(foo bar)");
    Parser *par = buildParser(input);

    AST::Expression *parsed = par->parse();
    if (parsed == 0)
    {
        error("unable to parse expression");
    }
}
}

void testParser()
{
    testInteger();
    testDecimal();
    testIdentifier();
    testCall();
}
