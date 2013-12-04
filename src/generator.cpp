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
        builder->CreateStore(argIt, alloca);
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

llvm::Value *Generator::generateConditional(AST::Conditional *conditional)
{
    llvm::Value *conditionValue = generateBool(conditional->getCondition());

    llvm::Function *fn = builder->GetInsertBlock()->getParent();

    llvm::BasicBlock *thenBlock =
        llvm::BasicBlock::Create(*context, "then", fn);
    llvm::BasicBlock *elseBlock =
        llvm::BasicBlock::Create(*context, "else");
    llvm::BasicBlock *mergeBlock =
        llvm::BasicBlock::Create(*context, "merge");

    builder->CreateCondBr(conditionValue, thenBlock, elseBlock);

    builder->SetInsertPoint(thenBlock);
    llvm::Value *thenValue = generate(conditional->getConsequent());
    builder->CreateBr(mergeBlock);
    thenBlock = builder->GetInsertBlock();

    fn->getBasicBlockList().push_back(elseBlock);
    builder->SetInsertPoint(elseBlock);
    llvm::Value *elseValue = generate(conditional->getAlternative());
    builder->CreateBr(mergeBlock);
    elseBlock = builder->GetInsertBlock();

    fn->getBasicBlockList().push_back(mergeBlock);
    builder->SetInsertPoint(mergeBlock);
    llvm::PHINode *phi = builder->CreatePHI(
        llvm::Type::getDoubleTy(*context),
        2,
        "iftmp"
    );
    phi->addIncoming(thenValue, thenBlock);
    phi->addIncoming(elseValue, elseBlock);

    return phi;
}
