#ifndef LEXER_H
#define LEXER_H

#include "../token/token.h"

#include <stdlib.h>

typedef struct {
    const char* buffer;
    TokenArray* tokens;
    size_t line;
    size_t len;
    size_t position;
    char character;
} Lexer;

Lexer* createLexer(const char* buffer);
void freeLexer(Lexer** lexer);
void lexerParse(Lexer* lexer);

#endif // !LEXER_H
