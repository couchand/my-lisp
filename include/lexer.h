// Lexer

#include <iostream>
#include <fstream>
#include <string>

enum Token
{
    token_eof = -1,
    token_identifier = -2,
    token_number = -3
};

class Lexer
{
    std::istream &input;
    std::string identifierStr;
    double numVal;

  public:
    Lexer(std::istream &_input) : input(_input) {}

    int getToken();
};
