// main

#include <iostream>

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/DataLayout.h"
#include "llvm/Transforms/Scalar.h"

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "generator.h"

Generator *generator;

int main(int argc, char **argv)
{
try
{
    double result;
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

    llvm::FunctionPassManager *passManager = new llvm::FunctionPassManager(generator->getModule());

    passManager->add(new llvm::DataLayout(*engine->getDataLayout()));
    passManager->add(llvm::createBasicAliasAnalysisPass());
    passManager->add(llvm::createInstructionCombiningPass());
    passManager->add(llvm::createReassociatePass());
    passManager->add(llvm::createGVNPass());
    passManager->add(llvm::createCFGSimplificationPass());

    passManager->doInitialization();

    while (1)
    {
        AST::Tree *tree = parser.parse();
        if (tree == 0) break;

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
            }
            else
            {
                std::cout << "unknown expression type in main" << std::endl;
                return 5;
            }
        }

        passManager->run(*fnTree);

        void *fnPtr = engine->getPointerToFunction(fnTree);
        if (fnPtr == 0)
        {
            std::cout << "unable to jit function";
            return 4;
        }

        double (*fn)() = (double (*)())(intptr_t)fnPtr;
        result = fn();
    }

    std::cout << "Evaluates to " << result << std::endl;
    return 0;
}
catch (const char* err)
{
    std::cout << "uncaught error: " << err << std::endl;
    return 3;
}

    return 0;
}
