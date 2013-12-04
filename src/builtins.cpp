// Builtins

#include <cstdio>
#include <cmath>

#include "builtins.h"

double add(double l, double r) { return l + r; }
double sub(double l, double r) { return l - r; }
double mul(double l, double r) { return l * r; }
double quo(double l, double r) { return l / r; }
double root(double x) { return sqrt(x); }
double print(double c) { putchar((char)c); return 0; }
double lt(double l, double r) { return l < r; }
