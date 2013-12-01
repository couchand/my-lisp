// Lexer

#include "lexer.h"

int Lexer::getToken()
{
    while (isspace(lastChar)) lastChar = input.get();

    if (input.eof()) return token_eof;

    if (isalpha(lastChar))
    {
        identifierStr = lastChar;
        while (isalnum(lastChar = input.get())) identifierStr += lastChar;
        return token_identifier;
    }

    if (isdigit(lastChar) || lastChar == '.')
    {
        std::string numStr;
        do
        {
            numStr += lastChar;
            lastChar = input.get();
        } while (isdigit(lastChar) || lastChar == '.');
        return token_number;
    }

    int thisChar = lastChar;
    lastChar = input.get();
    return thisChar;
}
