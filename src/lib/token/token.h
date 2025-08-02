#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    TokenTypeEof,
    TokenTypeIllegal,

    TokenTypeFn,
    TokenTypeReturn,
    TokenTypeLet,
    TokenTypePrint,
    TokenTypeNull,
    TokenTypeIf,
    TokenTypeElse,
    TokenTypeWhile,
    TokenTypeFor,

    TokenTypeBang,
    TokenTypeEquals,
    TokenTypeGreater,
    TokenTypeLess,

    TokenTypeEqualsEquals,
    TokenTypeBangEquals,
    TokenTypeGreaterEquals,
    TokenTypeLessEquals,

    TokenTypePlusPlus,
    TokenTypeMinusMinus,
    TokenTypePlusEquals,
    TokenTypeMinusEquals,
    TokenTypeStarEquals,
    TokenTypeSlashEquals,

    TokenTypeString,
    TokenTypeNumber,
    TokenTypeIdentifier,

    TokenTypeLeftParen,
    TokenTypeRightParen,
    TokenTypeLeftBrace,
    TokenTypeRightBrace,
    TokenTypeComma,
    TokenTypeDot,
    TokenTypePlus,
    TokenTypeMinus,
    TokenTypeStar,
    TokenTypeSlash,
    TokenTypeSemicolon,
    TokenTypeSingleQuote,
    TokenTypeDoubleQuote,
} TokenType;

typedef struct {
    TokenType type;
    char* literal;
    size_t line;
    bool shouldFree;
} Token;

typedef struct {
    Token* tokens;
    size_t count;
    size_t capacity;
} TokenArray;

Token createToken(TokenType type, char* literal, size_t line, bool shouldFree);
void freeToken(Token* token);

TokenArray* createTokenArray();
void freeTokenArray(TokenArray** arr);
void pushTokenArray(TokenArray* arr, Token token); 
Token* previousToken(TokenArray* arr);
void printTokenArray(TokenArray* arr); 

#endif // !TOKEN_H
