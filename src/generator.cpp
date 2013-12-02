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

void Generator::addParametersToScope(llvm::Function *fn, std::vector<std::string> parameters)
{
    llvm::Function::arg_iterator argIt = fn->arg_begin();
    for (
        unsigned i = 0;
        i != parameters.size();
        ++argIt, ++i
    ){
        argIt->setName(parameters[i]);
        addValue(parameters[i], argIt);
    }
}

void Generator::generateBody(llvm::Function *fn, AST::Expression *body)
{
    llvm::BasicBlock *block = llvm::BasicBlock::Create(*context, "entry", fn);
    builder->SetInsertPoint(block);

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
    if (fn == 0) throw "not found";
    return fn;
}

void Generator::addGlobal(std::string name, double val)
{
    addValue(name, generateGlobal(name, val));
}

llvm::GlobalVariable *Generator::generateGlobal(std::string name, double val)
{
    return new llvm::GlobalVariable(
        *module,
        llvm::Type::getDoubleTy(*context),
        true,
        llvm::GlobalValue::CommonLinkage,
        getConstant(val)
    );
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
