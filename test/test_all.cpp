// run all tests

#include <iostream>

extern void testLexer();

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
}
