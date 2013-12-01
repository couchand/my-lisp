// Generator

#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <string>
#include <vector>

#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/IRBuilder.h"

#include "types.h"
#include "builtins.h"
#include "ast.h"

class Generator
{
    llvm::LLVMContext *context;
    llvm::Module *module;
    Builder *builder;
    llvm::FunctionPassManager *passManager;

  public:
    Generator()
    {
        context = &llvm::getGlobalContext();
        builder = new Builder(*context);
        module = new llvm::Module("my-lisp", *context);

        buildFunction("add", 2);
    }

    Builder *getBuilder() { return builder; }
    llvm::Module *getModule() { return module; }
    llvm::LLVMContext *getContext() { return context; }
    llvm::Function *buildFunction(std::string name, unsigned parameters);
    llvm::Function *expressionToFunction(AST::Expression *expression);
    virtual llvm::Function *lookupFn(std::string name);

    virtual llvm::Value *generate(AST::Expression *expression);
};

#endif
