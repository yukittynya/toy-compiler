#ifndef LEXER_H
#define LEXER_H

#include "../token/token.h"

#define MAP_MAX 100

typedef struct {
    char keys[MAP_MAX][MAP_MAX];
    TokenType values[MAP_MAX];
    size_t count;
} KeywordMap;

int get_index(KeywordMap* map, char* key);
void insert(KeywordMap* map, char* key, TokenType value);
TokenType get(KeywordMap* map, char key[]);

typedef struct {
    char* buffer;
    TokenArray* tokens;
    KeywordMap keyword_map;
} Lexer;

Lexer* create_lexer(char* buffer);

#endif // !LEXER_H
