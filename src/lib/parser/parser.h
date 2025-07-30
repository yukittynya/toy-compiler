#ifndef PARSER_H
#define PARSER_H

#include "../token/token.h"

typedef struct {
    TokenArray* tokens;
} Parser;

Parser createParser(TokenArray* arr);

#endif // !define PARSER_H
