// Liebniz benchmark, iterative version

#include <cmath>

#include "builtins.h"

double term(double n)
{
    return quo(pow(-1, n), add(mul(2, n), 1));
}

double husk(double terms)
{
    double sofar = 0, n = 0;
    while (n < terms)
    {
        sofar = add(sofar, term(n));
        n = add(n, 1);
    }
    return mul(4, sofar);
}

int main(int argc, char **argv)
{
    for (int i = 0; i < 1000; ++i)
    {
        husk(100000);
    }
}
