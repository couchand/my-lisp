// Generator

#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

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
    std::map<std::string, llvm::Value*> scope;

  public:
    Generator()
    {
        context = &llvm::getGlobalContext();
        builder = new Builder(*context);
        module = new llvm::Module("my-lisp", *context);

        buildFunction("add", 2);
        buildFunction("print", 1);
        addGlobal("nil", 0);
    }

    Builder *getBuilder() { return builder; }
    llvm::Module *getModule() { return module; }
    llvm::LLVMContext *getContext() { return context; }

    llvm::Function *buildFunction(std::string name, unsigned parameters);
    llvm::Function *expressionToFunction(AST::Expression *expression);

    void addParametersToScope(llvm::Function *fn, std::vector<std::string> parameters);
    void generateBody(llvm::Function *fn, AST::Expression *body);
    void verifyFunction(llvm::Function *fn);

    llvm::ConstantFP *getConstant(double val);
    llvm::GlobalVariable *generateGlobal(std::string name, double val);
    llvm::Value *generateBool(AST::Expression *expression);
    llvm::Value *generateConditional(AST::Conditional *conditional);

    void addGlobal(std::string name, double val);
    void addValue(std::string name, llvm::Value *val);

    virtual llvm::Function *lookupFn(std::string name);
    virtual llvm::Value *lookupVal(std::string name);

    virtual llvm::Value *generate(AST::Expression *expression);
};

#endif
