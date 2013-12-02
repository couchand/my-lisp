// AST nodes

#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

#include "types.h"

namespace AST
{

    class Tree
    {
      public:
        virtual ~Tree() {}
    };

    class Expression : public Tree
    {
      public:
        virtual ~Expression() {}
        virtual llvm::Value *generate(Generator *generator) { return 0; }
    };

    class NumberLiteral : public Expression
    {
        double val;
      public:
        NumberLiteral(double _val) : val(_val) {}

        double getValue() { return val; }
        virtual llvm::Value *generate(Generator *generator);
    };

    class Identifier : public Expression
    {
        std::string name;
      public:
        Identifier(const std::string &_name) : name(_name) {}

        const char* getName() { return name.c_str(); }
        virtual llvm::Value *generate(Generator *generator);
    };

    class Call : public Expression
    {
        std::string name;
        std::vector<AST::Expression*> arguments;
      public:
        Call(const std::string &_name, const std::vector<AST::Expression*> &_arguments) : name(_name), arguments(_arguments) {}

        virtual llvm::Value *generate(Generator *generator);
    };

    class Function : public Tree
    {
        std::string name;
        std::vector<std::string> parameters;
        AST::Expression *body;
      public:
        Function(const std::string &_name, const std::vector<std::string> &_parameters, AST::Expression *_body) : name(_name), parameters(_parameters), body(_body) {}
        llvm::Function *generate(Generator *generator);
    };
}

#endif
