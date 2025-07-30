#include "lexer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Private 

typedef struct {
    const char* key;
    TokenType type;
} _Map;

_Map _keywordMap[] = {
    "fn", TokenTypeFn,
    "let", TokenTypeLet,
    "print", TokenTypePrint,
    "null", TokenTypeNull,
    "if", TokenTypeIf,
    "else", TokenTypeElse,
    "while", TokenTypeWhile,
    "for", TokenTypeFor,
};

_Map _compOperators[] = {
    "==", TokenTypeEqualsEquals,
    "!=", TokenTypeBangEquals,
    "<=", TokenTypeLessEquals,
    ">=", TokenTypeGreaterEquals,
};

size_t _keywordCount = sizeof(_keywordMap) / sizeof(_Map);
size_t _compOpCount = sizeof(_compOperators) / sizeof(_Map);

void _lexerReadChar(Lexer* lexer);
void _lexerAdvance(Lexer* lexer);
void _lexerSkipWhitespace(Lexer* lexer);
void _lexerMapString(Lexer* lexer, char* string);
void _lexerMapNumber(Lexer* lexer, char* string);
void _lexerMapNext(Lexer* lexer, char character);

bool _isAlpha(char c);
bool _isNumber(char c);

void _lexerReadChar(Lexer* lexer) {
    if (!lexer) return;

    if (lexer -> position >= lexer -> len) {
        lexer -> character = '\0';
    } else {
        lexer -> character = lexer -> buffer[lexer -> position];
    }

    if (lexer -> character == '\n') {
        lexer -> line++;
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

void _lexerMapString(Lexer* lexer, char* string) {
    char* literal = malloc(strlen(string) + 1);
    strcpy(literal, string);

    for (int i = 0; i < _keywordCount; i++) {
        if (strcmp(literal, _keywordMap[i].key) == 0) {
            Token token = createToken(_keywordMap[i].type, literal, lexer -> line);
            pushTokenArray(lexer -> tokens, token);
            return;
        }
    }

    Token* previous = previousToken(lexer -> tokens);

    if (previous -> type == TokenTypeLet || previous -> type == TokenTypeFn) {
        Token token = createToken(TokenTypeIdentifier, literal, lexer -> line);
        pushTokenArray(lexer -> tokens, token);
        return;
    }

    Token token = createToken(TokenTypeString, literal, lexer -> line);
    pushTokenArray(lexer -> tokens, token);
}

void _lexerMapNumber(Lexer* lexer, char* string) {
    char* literal = malloc(strlen(string) + 1);
    strcpy(literal, string);

    Token token = createToken(TokenTypeNumber, literal, lexer -> line);
    pushTokenArray(lexer -> tokens, token);
}

void _lexerMapNext(Lexer* lexer, char character) {
    char nextChar = lexer -> buffer[lexer -> position + 1]; 
    char* string = (char*) malloc(3);

    string[0] = character;
    string[1] = nextChar;
    string[2] = '\0';

    for (int i = 0; i < _compOpCount; i++) {
        if (strcmp(_compOperators[i].key, string) == 0) {
            Token token = createToken(_compOperators[i].type, string, lexer -> line);
            pushTokenArray(lexer -> tokens, token);

            _lexerAdvance(lexer);
            _lexerReadChar(lexer);
            return;
        }
    }
}

bool _isAlpha(char c) {
    if (c >= 65 && c <= 90) {
        return true;
    }

    if (c >= 97 && c <= 122) {
        return true;
    }

    return false;
}

bool _isNumber(char c) {
    if (c >= 48 && c <= 57) {
        return true;
    }

    return false;
}

// Public

void lexerParse(Lexer* lexer) {
    while (lexer -> position < lexer -> len) {
        _lexerSkipWhitespace(lexer);

        switch (lexer -> character) {
            case '\0':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeEof, "EOF", lexer -> line));
                break;

            case '(':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeLeftParen, "(", lexer -> line));
                break;

            case ')':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeRightParen, ")", lexer -> line));
                break;

            case '{':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeLeftBrace, "{", lexer -> line));
                break;

            case '}':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeRightBrace, "}", lexer -> line));
                break;

            case ',':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeComma, ",", lexer -> line));
                break;

            case '.':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeDot, ".", lexer -> line));
                break;

            case '+':
                pushTokenArray(lexer -> tokens, createToken(TokenTypePlus, "+", lexer -> line));
                break;
                
            case '-':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeMinus, "-", lexer -> line));
                break;

            case '*':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeStar, "*", lexer -> line));
                break;

            case '/':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeSlash, "/", lexer -> line));
                break;

            case ';':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeSemicolon, ";", lexer -> line));
                break;

            case '\'':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeSingleQuote, "'", lexer -> line));
                break;

            case '"':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeDoubleQuote, "\"", lexer -> line));
                break;

            case '=':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeEquals, "=", lexer -> line));
                break;

            default:
                break;
        }

        if (_isAlpha(lexer -> character)) {
            char string[1024];
            int i = 0;

            while (_isAlpha(lexer -> character)) {
                string[i++] = lexer -> character;

                _lexerAdvance(lexer);
                _lexerReadChar(lexer);
            }

            string[i] = '\0';
            _lexerMapString(lexer, string);
        } else if (_isNumber(lexer -> character)) {
            char string[1024];
            int i = 0;

            while (_isNumber(lexer -> character)) {
                string[i++] = lexer -> character;

                _lexerAdvance(lexer);
                _lexerReadChar(lexer);
            } 

            string[i] = '\0';

            _lexerMapNumber(lexer, string);
        } else {
            _lexerAdvance(lexer);
            _lexerReadChar(lexer);
        }
    }

    printTokenArray(lexer -> tokens);
}

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
    lexer -> tokens = createTokenArray();
    lexer -> len = strlen(buffer);
    lexer -> line = 1;
    lexer -> position = 0;

    _lexerReadChar(lexer);

    return lexer;
}

void freeLexer(Lexer** lexer) {
    if (!lexer || !*lexer) return;

    if (*lexer && (*lexer) -> tokens) {
        freeTokenArray(&(*lexer) -> tokens);
    }

    free(*lexer);
    *lexer = NULL;
} 
