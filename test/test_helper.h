// shared test functions

#ifndef TEST_HELPER__H
#define TEST_HELPER__H

#include <iostream>
#include <sstream>

#include "lexer.h"
#include "parser.h"

void error(const char* message);

void assertEquals(int expected, int actual, const char* message);

Lexer *buildLexer(std::stringstream &input);
Parser *buildParser(std::stringstream &input);

#endif
