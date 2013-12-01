// Parser

#include <vector>

#include "parser.h"

AST::Expression *Parser::parseNumber()
{
    AST::Expression *result = new AST::NumberLiteral(lexer->getLastNumber());
    getNextToken();
    return result;
}

AST::Expression *Parser::parseIdentifier()
{
    const char* name = lexer->getLastIdentifier();

    getNextToken();
    if (currentToken != '(')
    {
        return new AST::Identifier(name);
    }
    getNextToken();

    std::vector<AST::Expression*> arguments;
    while (currentToken != ')')
    {
        AST::Expression *argument = parse();
        arguments.push_back(argument);
    }
    getNextToken();

    return new AST::Call(name, arguments);
}

AST::Expression *Parser::parsePrimary()
{
    switch (currentToken)
    {
      default:
        printf("unknown token");
        throw "unknown token";
      case token_number:
        return parseNumber();
      case token_identifier:
        return parseIdentifier();
    }
}

AST::Expression *Parser::parse()
{
    return parsePrimary();
}
