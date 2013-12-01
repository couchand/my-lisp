// AST nodes

#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

namespace AST
{
    class Expression
    {
      public:
        virtual ~Expression() {}
    };

    class NumberLiteral : public Expression
    {
        double val;
      public:
        NumberLiteral(double _val) : val(_val) {}

        double getValue() { return val; }
    };

    class Identifier : public Expression
    {
        std::string name;
      public:
        Identifier(const std::string &_name) : name(_name) {}

        const char* getName() { return name.c_str(); }
    };

    class Call : public Expression
    {
        std::string name;
        std::vector<AST::Expression*> arguments;
      public:
        Call(const std::string &_name, const std::vector<AST::Expression*> &_arguments) : name(_name), arguments(_arguments) {}
    };
}

#endif
