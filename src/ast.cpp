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
    return value;
}

llvm::Value *Call::generate(Generator *generator)
{
    llvm::Function *fn = generator->lookupFn(name);
    if (fn == 0) throw "unable to find function object";

    if (fn->arg_size() != arguments.size()) throw "arity mismatch";

    std::vector<llvm::Value*> argv;
    for (unsigned i = 0, e = arguments.size(); i != e; ++i)
    {
        llvm::Value* parameter = arguments[i]->generate(generator);
        if (parameter == 0) throw "unable to generator parameter";

        argv.push_back(parameter);
    }

    return generator->getBuilder()->CreateCall(fn, argv, "calltmp");
}

llvm::Value *Conditional::generate(Generator *generator)
{
    return generator->generateConditional(this);
}

llvm::Function *Function::generate(Generator *generator)
{
    llvm::Function *fn = generator->buildFunction(name, parameters.size());

    if (fn->getName() != name)
    {
        throw "no redefs for now";
    }

    generator->addParametersToScope(fn, parameters);
    generator->generateBody(fn, body);
    generator->verifyFunction(fn);

    return fn;
}

}
