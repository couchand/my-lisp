// Lexer

#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <fstream>
#include <string>

enum Token
{
    token_eof = -1,
    token_identifier = -2,
    token_number = -3,
    token_define = -4,
    token_if = -5,
    token_then = -6,
    token_else = -7
};

class Lexer
{
    std::istream &input;
    std::string identifierStr;
    double numVal;
    int lastChar;

  public:
    Lexer(std::istream &_input) : input(_input), lastChar(' ') {}

    int getToken();
    double getLastNumber();
    const char* getLastIdentifier();
};

#endif
