// Lexer

#include "lexer.h"
int Lexer::getToken()
{
    static int lastChar = ' ';

    while (isspace(lastChar)) lastChar = getchar();

    if (lastChar == EOF) return token_eof;

    if (isalpha(lastChar))
    {
        identifierStr = lastChar;
        while (isalnum(lastChar = getchar())) identifierStr += lastChar;
        return token_identifier;
    }

    if (isdigit(lastChar) || lastChar == '.')
    {
        std::string numStr;
        do
        {
            numStr += lastChar;
            lastChar = getchar();
        } while (isdigit(lastChar) || lastChar == '.');
    }

    int thisChar = lastChar;
    lastChar = getchar();
    return thisChar;
}
