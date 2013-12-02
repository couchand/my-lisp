// Generator test

#include <iostream>
#include <sstream>

#include "test_helper.h"

namespace
{

void testInteger()
{
    std::stringstream input("42");
    AST::Expression *parsed = parse(input);
    if (parsed == 0)
    {
        error("unable to parse expression");
    }

    Generator generator = Generator();
    generator.generate(parsed)->dump();
}

void testCall()
{
    std::stringstream input("add(1 2)");
    AST::Expression *parsed = parse(input);
    if (parsed == 0)
    {
        error("unable to parse expression");
    }

    Generator generator = Generator();
    llvm::Value *value = generator.generate(parsed);
    value->dump();
}

void testConditional()
{
    std::stringstream input("define foobar(x) if 1 then 42 else 41");
    AST::Expression *parsed = parse(input);
    if (parsed == 0)
    {
        error("unable to parse expression");
    }

    Generator generator = Generator();
    llvm::Value *value = generator.generate(parsed);
    value->dump();
}

}

void testGenerator()
{
    testInteger();
    testCall();
    testConditional();
}
