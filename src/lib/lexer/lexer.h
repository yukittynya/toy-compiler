#ifndef LEXER_H
#define LEXER_H

#include "../token/token.h"

#include <stdlib.h>

typedef struct {
    const char* buffer;
    size_t len;
    size_t position;
    char character;
} Lexer;

Lexer* createLexer(const char* buffer);
void printTest(Lexer lexer); 

#endif // !LEXER_H
