#include "lexer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Private 

void _lexerReadChar(Lexer* lexer);
void _lexerAdvance(Lexer* lexer);
void _lexerSkipWhitespace(Lexer* lexer);

void _lexerReadChar(Lexer* lexer) {
    if (!lexer) return;

    if (lexer -> position >= lexer -> len) {
        lexer -> character = '\0';
    } else {
        lexer -> character = lexer -> buffer[lexer -> position];
    }
}

void _lexerAdvance(Lexer* lexer) {
    if (!lexer) return;

    if (lexer -> position < lexer -> len) {
        lexer -> position++;
    }
};

void _lexerSkipWhitespace(Lexer* lexer) {
    if (!lexer) return;

    while (lexer -> character == ' ' || lexer -> character == '\t' || lexer -> character == '\n') {
        if (lexer -> position >= lexer -> len) {
            return;
        }

        _lexerAdvance(lexer);
        _lexerReadChar(lexer);
    }
}


// Public

void printTest(Lexer* lexer) {
    while (lexer -> position < lexer->len) {  
        _lexerSkipWhitespace(lexer);

        printf("%c", lexer->character);

        _lexerAdvance(lexer);
        _lexerReadChar(lexer);  
    }
}

Lexer* createLexer(const char* buffer) {
    Lexer* lexer = calloc(1, sizeof(*lexer));

    if (!lexer) {
        return NULL;
    }

    lexer -> buffer = buffer;
    lexer -> len = strlen(buffer);
    lexer -> position = 0;

    _lexerReadChar(lexer);

    return lexer;
}
