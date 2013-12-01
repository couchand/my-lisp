// run all tests

#include <iostream>

extern void testLexer();
extern void testParser();
extern void testGenerator();

int main(int argc, char** argv)
{
    try
    {
        testLexer();
    }
    catch (const char* ex)
    {
        std::cout << "Exception: " << ex << std::endl;
    }
    try
    {
        testParser();
    }
    catch (const char* ex)
    {
        std::cout << "Exception: " << ex << std::endl;
    }
    try
    {
        testGenerator();
    }
    catch (const char* ex)
    {
        std::cout << "Exception: " << ex << std::endl;
    }
}
