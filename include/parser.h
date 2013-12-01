// Parser

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

class Parser
{
    int currentToken;
    Lexer *lexer;

    int getNextToken()
    {
        return currentToken = lexer->getToken();
    }

    AST::Expression *parseNumber();
    AST::Expression *parseIdentifier();
    AST::Expression *parsePrimary();

  public:
    Parser(Lexer *_lexer) : lexer(_lexer)
    {
        getNextToken();
    }

    AST::Expression *parse();
};

#endif
