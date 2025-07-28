#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>

typedef enum {
    TOK_FN,
    TOK_LET, 
    TOK_PRINT,
    TOK_NULL,

    TOK_STRING,
    TOK_IDENTIFIER,
    TOK_NUMBER,
    
    TOK_LEFT_PAREN,
    TOK_RIGHT_PAREN,
    TOK_LEFT_BRACE,
    TOK_RIGHT_BRACE,
    TOK_COMMA,
    TOK_DOT,
    TOK_STAR,
    TOK_SLASH,
    TOK_SEMICOLON,
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    size_t line;
    size_t col;
} Token;

typedef struct {
    Token* tokens;
    size_t count;
    size_t capacity;
} TokenArray;

const char* token_type_to_str(TokenType type);

// TokenArray

void token_array_init(TokenArray* arr); 
void token_array_push(TokenArray* arr, Token token);
void print_token_array(TokenArray* arr); 

// Token

Token create_token(TokenType type, char* value, size_t line, size_t col);
void print_token(Token token);

#endif // !TOKEN_H
