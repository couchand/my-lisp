// Generator

#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/IRBuilder.h"

#include "types.h"
#include "builtins.h"
#include "ast.h"

typedef std::function<llvm::Value*()> generateFn;

class Generator
{
    llvm::LLVMContext *context;
    llvm::Module *module;
    Builder *builder;
    std::map<std::string, llvm::Value*> scope;

  public:
    Generator()
    {
        context = &llvm::getGlobalContext();
        builder = new Builder(*context);
        module = new llvm::Module("my-lisp", *context);

        buildFunction("add", 2);
        buildFunction("sub", 2);
        buildFunction("mul", 2);
        buildFunction("quo", 2);
        buildFunction("pow", 2);
        buildFunction("root", 1);
        buildFunction("print", 1);
        buildFunction("lt", 2);
    }

    Builder *getBuilder() { return builder; }
    llvm::Module *getModule() { return module; }
    llvm::LLVMContext *getContext() { return context; }

    llvm::Function *buildFunction(std::string name, unsigned parameters);
    llvm::Function *expressionToFunction(AST::Expression *expression);
    llvm::Function *generateMain(std::vector<llvm::Function*> statements);
    llvm::Function *generatePredicate(std::string name, std::vector<std::string> parameters, std::vector<std::string> predicates);
    llvm::Function *getCurrentFunction();
    llvm::Value *generateCall(std::string name, std::vector<llvm::Value*> arguments);
    llvm::Value *generateMultiConditional(std::vector< std::pair<generateFn, generateFn> > cases, generateFn fallthrough);

    void addParametersToScope(llvm::Function *fn, std::vector<std::string> parameters);
    void generateBody(llvm::Function *fn, AST::Expression *body);
    void generateBody(llvm::Function *fn, llvm::BasicBlock *block, AST::Expression *body);
    void verifyFunction(llvm::Function *fn);

    llvm::ConstantFP *getConstant(double val);
    llvm::GlobalVariable *generateGlobal(std::string name, double val);
    llvm::Value *generateBool(AST::Expression *expression);
    llvm::Value *generateNotZero(llvm::Value *value);
    llvm::Value *generateConditional(AST::Conditional *conditional);
    llvm::AllocaInst *createEntryBlockAlloca(llvm::Function *fn, std::string name);
    llvm::BasicBlock *createEntryBlock(llvm::Function *fn);
    llvm::Value *generateLoad(std::string name, llvm::Value *alloca);
    llvm::Value *generateStore(llvm::Value *value, llvm::Value *alloca);

    void addGlobal(std::string name, double val);
    void addValue(std::string name, llvm::Value *val);
    llvm::Value *replace(std::string name, llvm::Value *newValue);

    virtual llvm::Function *lookupFn(std::string name);
    virtual llvm::Value *lookupVal(std::string name);

    virtual llvm::Value *generate(AST::Expression *expression);
};

#endif
