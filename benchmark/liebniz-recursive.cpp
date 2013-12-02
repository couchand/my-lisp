// Liebniz benchmark, recursive version

#include <cmath>

#include "builtins.h"

double term(double n)
{
    return quo(pow(-1, n), add(mul(2, n), 1));
}

double kernel(double sofar, double n, double max)
{
    if (n > max)
    {
        return sofar;
    }
    else
    {
        return kernel(
            add(sofar, term(n)),
            add(n, 1),
            max
        );
    }
}

double husk(double terms)
{
    return mul(4, kernel(0, 0, terms));
}

int main(int argc, char **argv)
{
    for (int i = 0; i < 1000; ++i)
    {
        husk(100000);
    }
}
