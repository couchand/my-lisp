// Parser test

#include <iostream>
#include <sstream>

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
}

void testParser()
{
    testInteger();
}
