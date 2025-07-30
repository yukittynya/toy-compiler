#ifndef PARSER_H
#define PARSER_H

#include "../token/token.h"

typedef enum {
    // functions decl, call, print, let, binary ops, identifier, string literal, number literal
} AstNodeType;

typedef struct {
    AstNodeType type;
    union {
        //statements
    } data;
} AstNode;

typedef struct {
    TokenArray* tokens;
} Parser;

Parser createParser(TokenArray* arr);

#endif // !define PARSER_H
