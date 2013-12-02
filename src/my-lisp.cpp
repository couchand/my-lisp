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
try
{
    Lexer lexer(std::cin);
    Parser parser(&lexer);
    generator = new Generator();

    llvm::InitializeNativeTarget();

    std::string errors;
    llvm::ExecutionEngine *engine = llvm::EngineBuilder(generator->getModule()).setErrorStr(&errors).create();
    if (!engine)
    {
        std::cout << "unable to initialize engine:  " << errors << std::endl;
        return 1;
    }

    while (1)
    {
        AST::Tree *tree = parser.parse();
        if (tree == 0) return 0;

        std::cout << "Parsed: " << std::endl;
        AST::Expression *expression = dynamic_cast<AST::Expression*>(tree);
        llvm::Function *fnTree;

        if (expression != 0)
        {
            fnTree = generator->expressionToFunction(expression);
            if (fnTree == 0)
            {
                std::cout << "unable to create function" << std::endl;
                return 2;
            }
            fnTree->dump();
        }
        else
        {
            AST::Function *fnAST = dynamic_cast<AST::Function*>(tree);

            if (fnAST != 0)
            {
                fnTree = fnAST->generate(generator);
                if (fnTree == 0)
                {
                    std::cout << "unable to create function" << std::endl;
                    return 2;
                }
                fnTree->dump();
            }
            else
            {
                std::cout << "unknown expression type in main" << std::endl;
                return 5;
            }
        }

        void *fnPtr = engine->getPointerToFunction(fnTree);
        if (fnPtr == 0)
        {
            std::cout << "unable to jit function";
            return 4;
        }

        double (*fn)() = (double (*)())(intptr_t)fnPtr;
        std::cout << "Evaluates to " << fn() << std::endl;
    }
}
catch (const char* err)
{
    std::cout << "uncaught error: " << err << std::endl;
    return 3;
}

    return 0;
}
