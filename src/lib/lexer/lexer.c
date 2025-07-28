#include "lexer.h"

#include <string.h>

int get_index(KeywordMap* map, char* key) {
    for (int i = 0; i < map -> count; i++) {
        if (strcmp(map -> keys[i], key) == 0) {
            return i;
        }
    }

    return -1;
}

void insert(KeywordMap *map, char *key, TokenType value) {
    int index = get_index(map, key);

    if (index == -1) {
        strcpy(map -> keys[map -> count], key);

        map -> values[map -> count] = value;
        map -> count++;
    } else {
        map -> values[index] = value;
    }
}

TokenType get(KeywordMap* map, char* key) {
    int index = get_index(map, key);

    if (index == -1) {
        return TOK_NULL;
    } else {
        return map -> values[index];
    }
}

Lexer* create_lexer(char* buffer) {
    Lexer* lexer = (Lexer*) malloc(sizeof(Lexer));

    lexer -> buffer = (char*) malloc(strlen(buffer) + 1);
    strcpy(lexer -> buffer, buffer);
    
    token_array_init(lexer -> tokens);

    insert(&lexer -> keyword_map, "fn", TOK_FN);
    insert(&lexer -> keyword_map, "let", TOK_LET);
    insert(&lexer -> keyword_map, "print", TOK_PRINT);

    return lexer;
}
