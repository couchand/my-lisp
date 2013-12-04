#include "generator.h"

#include <iostream>
#include <vector>

#include "llvm/Analysis/Verifier.h"

llvm::ConstantFP *Generator::getConstant(double val)
{
    return llvm::ConstantFP::get(*context, llvm::APFloat(val));
}

llvm::Function *Generator::expressionToFunction(AST::Expression *expression)
{
    llvm::Function *fn = buildFunction("", 0);
    generateBody(fn, expression);
    verifyFunction(fn);

    return fn;
}

llvm::Function *Generator::generateMain(std::vector<llvm::Function*> statements)
{
    std::vector<llvm::Type*> none;
    std::vector<llvm::Value*> empty;
    llvm::Function *main = llvm::Function::Create(
        llvm::FunctionType::get(llvm::Type::getVoidTy(*context), none, false),
        llvm::Function::ExternalLinkage,
        "main",
        module
    );

    llvm::BasicBlock *block = llvm::BasicBlock::Create(*context, "entry", main);
    builder->SetInsertPoint(block);

    for (std::vector<llvm::Function*>::iterator i = statements.begin(); i != statements.end(); ++i)
    {
        builder->CreateCall(*i, empty, "calltmp");
    }

    builder->CreateRetVoid();
    verifyFunction(main);

    return main;
}

llvm::Function *Generator::generatePredicate(std::string name, std::vector<std::string> parameters, std::vector<std::string> predicates)
{
    llvm::Function *pred = buildFunction(name + "$predicate", parameters.size());

    createEntryBlock(pred);
    addParametersToScope(pred, parameters);

    llvm::Value* results = getConstant(1);
    std::vector<llvm::Value*> argv;
    for (unsigned i = 0, e = predicates.size(); i != e; ++i)
    {
        if (predicates[i].empty()) continue;

        llvm::Value *value = lookupVal(parameters[i]);
        if (!value) throw "something strange occurred";
        llvm::Value* parameter = generateLoad(parameters[i], value);
        if (parameter == 0) throw "unable to generate parameter load";

        argv.clear();
        argv.push_back(parameter);

        results = builder->CreateAnd(results, generateCall(predicates[i], argv), "andtmp");
    }
    builder->CreateRet(results);

    verifyFunction(pred);

    return pred;
}

llvm::Value *Generator::generateCall(std::string name, std::vector<llvm::Value*> arguments)
{
    llvm::Function *fn = lookupFn(name);
    if (fn == 0) throw "unable to find function object";

    if (fn->arg_size() != arguments.size()) throw "arity mismatch";

    return builder->CreateCall(fn, arguments, "calltmp");
}

llvm::Function *Generator::getCurrentFunction()
{
    return builder->GetInsertBlock()->getParent();
}

llvm::Value *Generator::generateStore(llvm::Value* value, llvm::Value* alloca)
{
        return builder->CreateStore(value, alloca);
}

void Generator::addParametersToScope(llvm::Function *fn, std::vector<std::string> parameters)
{
    llvm::Function::arg_iterator argIt = fn->arg_begin();
    for (
        unsigned i = 0;
        i != parameters.size();
        ++argIt, ++i
    ){
        argIt->setName(parameters[i]);
        llvm::AllocaInst *alloca = createEntryBlockAlloca(fn, parameters[i]);
        generateStore(argIt, alloca);
        addValue(parameters[i], alloca);
    }
}

llvm::AllocaInst *Generator::createEntryBlockAlloca(llvm::Function *fn, std::string name)
{
    Builder entryBuilder(&fn->getEntryBlock(), fn->getEntryBlock().begin());
    return entryBuilder.CreateAlloca(
        llvm::Type::getDoubleTy(*context),
        0,
        name.c_str()
    );
}

llvm::Value *Generator::generateLoad(std::string name, llvm::Value *alloca)
{
    return builder->CreateLoad(alloca, name.c_str());
}

llvm::BasicBlock *Generator::createEntryBlock(llvm::Function *fn)
{
    llvm::BasicBlock *block = llvm::BasicBlock::Create(*context, "entry", fn);
    builder->SetInsertPoint(block);
    return block;
}

void Generator::generateBody(llvm::Function *fn, AST::Expression *body)
{
    generateBody(fn, createEntryBlock(fn), body);
}

void Generator::generateBody(llvm::Function *fn, llvm::BasicBlock *block, AST::Expression *body)
{
    llvm::Value *value = generate(body);
    builder->CreateRet(value);
}

void Generator::verifyFunction(llvm::Function *fn)
{
    llvm::verifyFunction(*fn);
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
    if (fn == 0) throw (name + " not found").c_str();
    return fn;
}

void Generator::addValue(std::string name, llvm::Value *val)
{
    scope[name] = val;
}

llvm::Value *Generator::lookupVal(std::string name)
{
    llvm::Value *value = scope[name];
    if (!value) throw "unknown value";
    return value;
}

llvm::Value *Generator::replace(std::string name, llvm::Value *newValue)
{
    llvm::Value* oldValue = scope[name];
    scope[name] = newValue;
    return oldValue;
}

llvm::Value *Generator::generate(AST::Expression *expression)
{
    return expression->generate(this);
}

llvm::Value *Generator::generateBool(AST::Expression *expression)
{
    llvm::Value *value = expression->generate(this);

    return builder->CreateFCmpONE(
        value,
        llvm::ConstantFP::get(*context, llvm::APFloat(0.0)),
        "ifcond"
    );
}

llvm::Value *Generator::generateMultiConditional(std::vector< std::pair<generateFn, generateFn> > cases, generateFn fallthrough)
{
    if (cases.empty()) return fallthrough();

    for (unsigned i = 0; i < cases.size() - 1; ++i)
    {
        throw "no multi yet";
    }

    llvm::Value *conditionValue = cases[cases.size() - 1].first();

    llvm::Function *fn = builder->GetInsertBlock()->getParent();

    llvm::BasicBlock *thenBlock =
        llvm::BasicBlock::Create(*context, "then", fn);
    llvm::BasicBlock *elseBlock =
        llvm::BasicBlock::Create(*context, "else");
    llvm::BasicBlock *mergeBlock =
        llvm::BasicBlock::Create(*context, "merge");

    builder->CreateCondBr(conditionValue, thenBlock, elseBlock);

    builder->SetInsertPoint(thenBlock);
    llvm::Value *thenValue = cases[cases.size() - 1].second();
    builder->CreateBr(mergeBlock);
    thenBlock = builder->GetInsertBlock();

    fn->getBasicBlockList().push_back(elseBlock);
    builder->SetInsertPoint(elseBlock);
    llvm::Value *elseValue = fallthrough();
    builder->CreateBr(mergeBlock);
    elseBlock = builder->GetInsertBlock();

    fn->getBasicBlockList().push_back(mergeBlock);
    builder->SetInsertPoint(mergeBlock);
    llvm::PHINode *phi = builder->CreatePHI(
        llvm::Type::getDoubleTy(*context),
        cases.size() + 1,
        "iftmp"
    );
    phi->addIncoming(thenValue, thenBlock);
    phi->addIncoming(elseValue, elseBlock);

    return phi;
}

llvm::Value *Generator::generateConditional(AST::Conditional *conditional)
{
    std::vector< std::pair<generateFn, generateFn> > cases;
    std::pair<generateFn, generateFn> then;

    then.first = [&]() { return generateBool(conditional->getCondition()); };
    then.second = [&]() { return generate(conditional->getConsequent()); };

    cases.push_back(then);

    generateFn fallthrough = [&]() { return  generate(conditional->getAlternative()); };

    return generateMultiConditional(cases, fallthrough);
}
