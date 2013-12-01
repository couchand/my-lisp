// main

#include <iostream>

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/Support/TargetSelect.h"

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "generator.h"

Generator *generator;

int main(int argc, char **argv)
{
    Lexer lexer(std::cin);
    Parser parser(&lexer);
    generator = new Generator();

    AST::Expression *expression = parser.parse();

    llvm::InitializeNativeTarget();

    std::string errors;
    llvm::ExecutionEngine *engine = llvm::EngineBuilder(generator->getModule()).setErrorStr(&errors).create();
    if (!engine)
    {
        std::cout << "unable to initialize engine:  " << errors << std::endl;
        return 1;
    }

    llvm::Function *fnTree = generator->expressionToFunction(expression);
    if (fnTree == 0)
    {
        std::cout << "unable to create function" << std::endl;
        return 2;
    }

    std::cout << "Parsed: " << std::endl;
    fnTree->dump();

    void *fnPtr = engine->getPointerToFunction(fnTree);

    double (*fn)() = (double (*)())(intptr_t)fnPtr;
    std::cout << "Evaluates to " << fn() << std::endl;

    return 0;
}
