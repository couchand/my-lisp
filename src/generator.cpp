#include "generator.h"

#include <iostream>
#include <vector>

void Util::registerBuiltin(std::string name, llvm::Module *module, llvm::LLVMContext *context)
{
    std::vector<llvm::Type*> doubles(
        2,
        llvm::Type::getDoubleTy(*context)
    );

    llvm::Function *add = llvm::Function::Create(
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
