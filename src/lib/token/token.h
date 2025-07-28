#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
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

typedef struct _Token {
    TokenType type;
    char* literal;
} *Token;

Token createToken(TokenType type, char* literal);
void freeToken(Token* tok);

#endif // !TOKEN_H
