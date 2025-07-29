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
    TokenTypeSemicolon,
    TokenTypeSingleQuote,
    TokenTypeDoubleQuote,
    TokenTypeEquals,
} TokenType;

typedef struct {
    TokenType type;
    char* literal;
    size_t line;
} Token;

typedef struct {
    Token* tokens;
    size_t count;
    size_t capacity;
} TokenArray;

Token createToken(TokenType type, char* literal, size_t line);

TokenArray* createTokenArray();
void freeTokenArray(TokenArray** arr);
void pushTokenArray(TokenArray* arr, Token token); 
Token* previousToken(TokenArray* arr);
void printTokenArray(TokenArray* arr); 

#endif // !TOKEN_H
