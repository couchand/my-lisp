#include "generator.h"

#include <iostream>
#include <vector>

#include "llvm/Analysis/Verifier.h"

llvm::Function *Generator::expressionToFunction(AST::Expression *expression)
{
    llvm::Function *fn = buildFunction("", 0);

    llvm::BasicBlock *block = llvm::BasicBlock::Create(*context, "entry", fn);
    builder->SetInsertPoint(block);

    llvm::Value *value = generate(expression);
    builder->CreateRet(value);

    llvm::verifyFunction(*fn);
    return fn;
}

llvm::Function *Generator::buildFunction(std::string name, unsigned parameters)
{
    std::vector<llvm::Type*> doubles(
        parameters,
        llvm::Type::getDoubleTy(*context)
    );

    return llvm::Function::Create(
        llvm::FunctionType::get(
            llvm::Type::getDoubleTy(*context),
            doubles,
            false
        ),
        llvm::Function::ExternalLinkage,
        name,
        module
    );
}

llvm::Function *Generator::lookupFn(std::string name)
{
    llvm::Function *fn = module->getFunction(name);
    if (fn == 0) throw "not found";
    return fn;
}

llvm::Value *Generator::generate(AST::Expression *expression)
{
    return expression->generate(this);
}
