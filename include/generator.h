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

namespace Util
{
    void registerBuiltin(std::string name, llvm::Module *module, llvm::LLVMContext *context);
}

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

        Util::registerBuiltin("add", module, context);
    }

    Builder *getBuilder() { return builder; }
    virtual llvm::Function *lookupFn(std::string name);

    virtual llvm::Value *generate(AST::Expression *expression);
};

#endif
