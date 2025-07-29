#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* _typeToString(TokenType type) {
    switch (type) {
        case TokenTypeEof:          return "TokenTypeEof";
        case TokenTypeIllegal:      return "TokenTypeIllegal";
        case TokenTypeFn:           return "TokenTypeFn";
        case TokenTypeLet:          return "TokenTypeLet";
        case TokenTypePrint:        return "TokenTypePrint";
        case TokenTypeNull:         return "TokenTypeNull";

        case TokenTypeString:       return "TokenTypeString";
        case TokenTypeNumber:       return "TokenTypeNumber";
        case TokenTypeIdentifier:   return "TokenTypeIdentifier";

        case TokenTypeLeftParen:    return "TokenTypeLeftParen";
        case TokenTypeRightParen:   return "TokenTypeRightParen";
        case TokenTypeLeftBrace:    return "TokenTypeLeftBrace";
        case TokenTypeRightBrace:   return "TokenTypeRightBrace";
        case TokenTypeComma:        return "TokenTypeComma";
        case TokenTypeDot:          return "TokenTypeDot";
        case TokenTypeStar:         return "TokenTypeStar";
        case TokenTypeSlash:        return "TokenTypeSlash";
        case TokenTypeSemicolon:    return "TokenTypeSemicolon";
        case TokenTypeSingleQuote:  return "TokenTypeSingleQuote";
        case TokenTypeDoubleQuote:  return "TokenTypeDoubleQuote";
        default:                    return "Unknown";
    }
}

void _printToken(Token* tok) {
    if (!tok) return;

    printf("Token: [Type: %s, Literal: \"%s\", Line: %zu]\n", _typeToString(tok -> type), tok -> literal, tok -> line);
}

TokenArray* createTokenArray() {
    TokenArray* arr = calloc(1, sizeof(TokenArray));

    if (!arr) {
        return NULL;
    }

    arr -> count = 0;
    arr -> capacity = 32;
    arr -> tokens = calloc(arr -> capacity, sizeof(Token));

    if (!arr -> tokens) {
        free(arr);
        return NULL;
    }

    return arr;
}

void freeTokenArray(TokenArray** arr) {
    if (!arr || !arr) return;

    if (*arr && (*arr) -> tokens) {
        free((*arr) -> tokens);
    }   

    free(*arr);
    *arr = NULL;
}

void pushTokenArray(TokenArray* arr, Token token) {
    if (!arr) {
        return;
    }

    if (arr -> count >= arr -> capacity) {
        arr -> capacity *= 2;
        Token* new_arr = realloc(arr -> tokens, sizeof(Token) * arr -> capacity);
        
        if (!new_arr) {
            fprintf(stderr, "ERROR: Failed to realloc token array (capacity: %zu)\n", arr -> capacity);
            exit(1);
        }

        arr -> tokens = new_arr;
    }

    arr -> tokens[arr -> count] = token;
    arr -> count++;
}

void printTokenArray(TokenArray* arr) {
    if (!arr) return;
    
    for (int i = 0; i < arr -> count; i++) {
        _printToken(&arr -> tokens[i]);
    }
}


Token createToken(TokenType type, char* literal, size_t line) {
    Token tok;

    tok.type = type;
    tok.literal = literal;
    tok.line = line;

    return tok;
}
