#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>

typedef enum {
    TokenTypeEof,
    TokenTypeIllegal,

    TokenTypeFn,
    TokenTypeLet,
    TokenTypePrint,
    TokenTypeNull,

    TokenTypeString,
    TokenTypeNumber,
    TokenTypeIdentifier,

    TokenTypeLeftParen,
    TokenTypeRightParen,
    TokenTypeLeftBrace,
    TokenTypeRightBrace,
    TokenTypeComma,
    TokenTypeDot,
    TokenTypeStar,
    TokenTypeSlash,
    TokenTypeSemicolon
} TokenType;

typedef struct {
    TokenType type;
    char* literal;
} Token;

typedef struct {
    Token* tokens;
    size_t count;
    size_t capacity;
} TokenArray;

Token* createToken(TokenType type, char* literal);
void freeToken(Token** tok);

TokenArray* createTokenArray();
void freeTokenArray(TokenArray** arr);
void pushTokenArray(TokenArray* arr, Token* token); 

#endif // !TOKEN_H
