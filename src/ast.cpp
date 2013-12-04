#include "ast.h"
#include "generator.h"

#include <iostream>

#include "llvm/ADT/APFloat.h"
#include "llvm/DerivedTypes.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/IRBuilder.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/DataLayout.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/TargetSelect.h"

namespace AST
{

llvm::Value *NumberLiteral::generate(Generator *generator)
{
    return generator->getConstant(val);
}

llvm::Value *Identifier::generate(Generator *generator)
{
    llvm::Value *value = generator->lookupVal(name);
    if (!value) throw "unknown name";
    return generator->generateLoad(name, value);
}

llvm::Value *Call::generate(Generator *generator)
{
    std::vector<llvm::Value*> argv;
    for (unsigned i = 0, e = arguments.size(); i != e; ++i)
    {
        llvm::Value* parameter = generator->generate(arguments[i]);
        if (parameter == 0) throw "unable to generate parameter";

        argv.push_back(parameter);
    }

    return generator->generateCall(name, argv);
}

llvm::Value *Conditional::generate(Generator *generator)
{
    return generator->generateConditional(this);
}

llvm::Value *Let::generate(Generator *generator)
{
    std::vector<llvm::Value*> oldScope;

    llvm::Function *fn = generator->getCurrentFunction();

    for (unsigned i = 0; i < assignments.size(); ++i)
    {
        const std::string &name = assignments[i].first;
        AST::Expression *initializer = assignments[i].second;

        llvm::Value *initial;
        if (initializer)
        {
            initial = generator->generate(initializer);
            if (initial == 0) throw "unable to generate initializer";
        }
        else
        {
            initial = generator->getConstant(0.0);
        }

        llvm::AllocaInst *alloca = generator->createEntryBlockAlloca(fn, name);
        generator->generateStore(initial, alloca);

        oldScope.push_back(generator->replace(name, alloca));
    }

    llvm::Value *bodyVal = generator->generate(body);
    if (body == 0) throw "unable to generate body";

    for (unsigned i = 0; i < assignments.size(); ++i)
    {
        generator->replace(assignments[i].first, oldScope[i]);
    }

    return bodyVal;
}

llvm::Function *Function::generate(Generator *generator)
{
    if (fn->getName() != name)
    {
        throw "no redefs for now";
    }

    std::vector<std::string> parameterNames;
    for (unsigned i = 0; i < parameters.size(); ++i)
    {
        parameterNames.push_back(parameters[i].first);
    }

    llvm::Function *fn = generator->buildFunction(name, parameters.size());

    llvm::BasicBlock *entry = generator->createEntryBlock(fn);
    generator->addParametersToScope(fn, parameterNames);
    generator->generateBody(fn, entry, body);
    generator->verifyFunction(fn);

    return fn;
}

}
