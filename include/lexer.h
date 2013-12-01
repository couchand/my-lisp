// Lexer

#include <cstdio>
#include <string>

enum Token
{
    token_eof = -1,
    token_identifier = -2,
    token_number = -3
};

class Lexer
{
    std::string identifierStr;
    double numVal;

    int getToken();
};
