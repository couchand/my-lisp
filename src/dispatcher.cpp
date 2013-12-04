#include "dispatcher.h"
#include "generator.h"

void Dispatcher::registerMethod(std::string name, llvm::Function *fn, llvm::Function *pred)
{
    std::pair<llvm::Function*, llvm::Function*> binding(fn, pred);
    methods[name].push_back(binding);
}

llvm::Function *Dispatcher::dispatch(std::string name, std::vector<llvm::Value*> arguments)
{
    std::cout << "building dispatch function " << name << std::endl;
    std::cout << "airity " << arguments.size() << std::endl;
    std::cout << "candidates " << methods[name].size() << std::endl;

    llvm::Function *ret = generator->buildFunction(name, arguments.size());

    std::vector<llvm::Value*> parameters;
    llvm::Function::arg_iterator argIt = ret->arg_begin();
    for (unsigned i = 0; i != arguments.size(); ++argIt, ++i)
    {
        parameters.push_back(argIt);
    }

    std::cout << "function built" << std::endl;
    generator->createEntryBlock(ret);
    std::cout << "entry block created" << std::endl;

    std::vector< std::pair<generateFn, generateFn> > cases;
    std::vector< std::pair<llvm::Function*, llvm::Function*> > candidates = methods[name];

    for (unsigned i = 0; i < candidates.size(); ++i)
    {
        std::cout << "queuing candidate" << i << std::endl;
        generateFn condition = [this, &candidates, &parameters, i]() { return generator->generateCall(candidates[i].second, parameters); };
        generateFn consequent = [&candidates, i]() { return candidates[i].first; };
        cases.push_back(std::pair<generateFn, generateFn>(condition, consequent));
    }
    std::cout << "queuing fallthrough" << std::endl;
    generateFn fallthrough = [this]() { return generator->getConstant(0); };

    std::cout << "building multicondition" << std::endl;
    llvm::Value *multi = generator->generateMultiConditional(cases, fallthrough);
    std::cout << "building return" << std::endl;
    generator->generateReturn(multi);

    std::cout << "verifying..." << std::endl;
    generator->verifyFunction(ret);

    return ret;
}
