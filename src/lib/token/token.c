#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* _typeToString(TokenType type) {
    switch (type) {
        case TokenTypeIllegal:     return "TokenTypeIllegal";
        case TokenTypeFn:          return "TokenTypeFn";
        case TokenTypeLet:         return "TokenTypeLet";
        case TokenTypePrint:       return "TokenTypePrint";
        case TokenTypeNull:        return "TokenTypeNull";
        case TokenTypeString:      return "TokenTypeString";
        case TokenTypeNumber:      return "TokenTypeNumber";
        case TokenTypeIdentifier:  return "TokenTypeIdentifier";
        case TokenTypeLeftParen:   return "TokenTypeLeftParen";
        case TokenTypeRightParen:  return "TokenTypeRightParen";
        case TokenTypeLeftBrace:   return "TokenTypeLeftBrace";
        case TokenTypeRightBrace:  return "TokenTypeRightBrace";
        case TokenTypeComma:       return "TokenTypeComma";
        case TokenTypeDot:         return "TokenTypeDot";
        case TokenTypeStar:        return "TokenTypeStar";
        case TokenTypeSlash:       return "TokenTypeSlash";
        case TokenTypeSemicolon:   return "TokenTypeSemicolon";
        default:                   return "Unknown";
    }
}

void _printToken(Token tok) {
    if (!tok) return;

    printf("Token: [Type %s, Literal: %s]", _typeToString(tok -> type), tok -> literal);
}

Token createToken(TokenType type, char* literal) {
    Token tok; 
    size_t len = sizeof(*tok);

    tok = malloc(len);
    if (!tok) {
        return NULL;
    }

    memset(tok, 0, len);

    tok -> type = type;
    tok -> literal = literal;

    return tok;
}

void freeToken(Token* tok) {
    if (!tok || !*tok) return;

    if (*tok && (*tok) -> literal)  {
        free((*tok) -> literal);
    }

    free(*tok);
    *tok = NULL;
}
