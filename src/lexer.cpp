// Lexer

#include <cstdlib>

#include "lexer.h"

int Lexer::getToken()
{
    while (isspace(lastChar)) lastChar = input.get();

    if (input.eof()) return token_eof;

    if (lastChar == '#')
    {
        while (lastChar != '\n')
        {
            lastChar = input.get();
        }
        return getToken();
    }

    if (isalpha(lastChar))
    {
        identifierStr = lastChar;
        while (isalnum(lastChar = input.get())) identifierStr += lastChar;

        if (identifierStr == "define") return token_define;
        if (identifierStr == "if") return token_if;
        if (identifierStr == "then") return token_then;
        if (identifierStr == "else") return token_else;
        if (identifierStr == "let") return token_let;
        if (identifierStr == "in") return token_in;

        return token_identifier;
    }

    if (isdigit(lastChar) || lastChar == '.' || lastChar == '-')
    {
        std::string numStr;
        do
        {
            numStr += lastChar;
            lastChar = input.get();
        } while (isdigit(lastChar) || lastChar == '.');

        numVal = strtod(numStr.c_str(), 0);
        return token_number;
    }

    int thisChar = lastChar;
    lastChar = input.get();
    return thisChar;
}

double Lexer::getLastNumber()
{
    return numVal;
}

const char* Lexer::getLastIdentifier()
{
    return identifierStr.c_str();
}
