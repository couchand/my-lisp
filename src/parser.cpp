// Parser

#include "parser.h"

AST::Expression *Parser::parseNumber()
{
    AST::Expression *result = new AST::NumberLiteral(lexer->getLastNumber());
    getNextToken();
    return result;
}

AST::Expression *Parser::parseIdentifier()
{
    AST::Expression *result = new AST::Identifier(lexer->getLastIdentifier());
    getNextToken();
    return result;
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
