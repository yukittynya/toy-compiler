#ifndef LEXER_H
#define LEXER_H

#include "../token/token.h"

typedef struct {
    const char* file_path;
    TokenArray* tokens;
} Lexer;

#endif // !LEXER_H
