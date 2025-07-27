#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void token_array_init(TokenArray *arr) {
    arr -> tokens = (Token*) malloc(sizeof(Token) * 32);
    arr -> count = 0;
    arr -> capacity = 16;
}

void token_array_push(TokenArray* arr, Token token) {
    if (arr -> count >= arr -> capacity) {
        arr -> capacity *= 2;
        arr -> tokens = realloc(arr -> tokens, sizeof(Token) * arr -> capacity);
    }

    arr -> tokens[arr -> count] = token;
    arr -> count++;
}

void print_token_array(TokenArray* arr) {
    for (int i = 0; i < arr -> count; i++) {
        print_token(arr -> tokens[i]);
    }
}

Token create_token(TokenType type, char *value, size_t line, size_t col) {
    Token token;

    token.type = type;
    strcpy(token.value, value);
    token.line = line;
    token.col = col;

    return token;
}

void print_token(Token token) {
    const char* type = token_type_to_str(token.type); 

    printf("Token [Type: %s, Value: %s, Line: %zu, Col: %zu]\n", type, token.value, token.line, token.col);
}

const char* token_type_to_str(TokenType type) {
    switch (type) {
        case TOK_FN:
            return "TOK_FN";
            break;

        case TOK_LET: 
            return "TOK_LET";
            break;

        case TOK_PRINT:
            return "TOK_PRINT";
            break;

        case TOK_STRING:
            return "TOK_STRING";
            break;
        case TOK_IDENTIFIER:
            return "TOK_IDENTIFIER";
            break;
        case TOK_NUMBER:
            return "TOK_NUMBER";
            break;

        case TOK_LEFT_PAREN:
            return "TOK_LEFT_PAREN";
            break;

        case TOK_RIGHT_PAREN:
            return "TOK_RIGHT_PAREN";
            break;

        case TOK_LEFT_BRACE:
            return "TOK_LEFT_BRACE";
            break;

        case TOK_RIGHT_BRACE:
            return "TOK_RIGHT_BRACE";
            break;

        case TOK_COMMA:
            return "TOK_COMMA";
            break;

        case TOK_DOT:
            return "TOK_DOT";
            break;

        case TOK_STAR:
            return "TOK_STAR";
            break;

        case TOK_SLASH:
            return "TOK_SLASH";
            break;

        case TOK_SEMICOLON:
            return "TOK_SEMICOLON";
            break;

        default:
            return NULL;
            break;
    }
}
