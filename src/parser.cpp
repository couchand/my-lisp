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
    std::string name = lexer->getLastIdentifier();
    getNextToken();

    if (currentToken != '(')
    {
        return new AST::Identifier(name);
    }
    getNextToken();

    std::vector<AST::Expression*> arguments;
    while (currentToken != ')')
    {
        AST::Expression *argument = parsePrimary();
        arguments.push_back(argument);
    }
    getNextToken();

    return new AST::Call(name, arguments);
}

AST::Function *Parser::parseDefine()
{
    if (getNextToken() != token_identifier) throw "expected identifier";
    std::string name = lexer->getLastIdentifier();
    getNextToken();

    if (currentToken != '(') throw "expected parameters";

    std::vector<std::string> parameters;
    while (getNextToken() != ')')
    {
        if (currentToken != token_identifier) throw "expecting identifier";
        parameters.push_back(lexer->getLastIdentifier());
    }
    getNextToken();

    AST::Expression *body = parsePrimary();

    return new AST::Function(name, parameters, body);
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

AST::Expression *Parser::parseExpression()
{
    return parsePrimary();
}

AST::Tree *Parser::parse()
{
    switch (currentToken)
    {
      default:
        return parsePrimary();
      case token_eof:
        return 0;
      case token_define:
        return parseDefine();
    }
}
