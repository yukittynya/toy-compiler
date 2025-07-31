#include "lexer.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Private 

typedef struct {
    const char* key;
    TokenType type;
} _Map;

static const _Map _keywordMap[] = {
    "fn", TokenTypeFn,
    "return", TokenTypeReturn,
    "let", TokenTypeLet,
    "print", TokenTypePrint,
    "null", TokenTypeNull,
    "if", TokenTypeIf,
    "else", TokenTypeElse,
    "while", TokenTypeWhile,
    "for", TokenTypeFor,
};

static const _Map _operators[] = {
    "==", TokenTypeEqualsEquals,
    "!=", TokenTypeBangEquals,
    "<=", TokenTypeLessEquals,
    ">=", TokenTypeGreaterEquals,

    "++", TokenTypePlusPlus,
    "--", TokenTypeMinusMinus,
    "+=", TokenTypePlusEquals,
    "-=", TokenTypeMinusEquals,
    "*=", TokenTypeStarEquals,
    "/=", TokenTypeSlashEquals,
};

static const size_t _keywordCount = sizeof(_keywordMap) / sizeof(_Map);
static const size_t _operatorCount = sizeof(_operators) / sizeof(_Map);

static const uint8_t _charMap[256] = {
    [0 ... 31] = 0,

    [' '] = 1, ['\t'] = 1, ['\n'] = 1,

    [33 ... 47] = 0,

    ['0'] = 2, ['1'] = 2, ['2'] = 2, ['3'] = 2, ['4'] = 2,
    ['5'] = 2, ['6'] = 2, ['7'] = 2, ['8'] = 2, ['9'] = 2,

    [58 ... 64] = 0,

    ['A'] = 4, ['B'] = 4, ['C'] = 4, ['D'] = 4, ['E'] = 4, ['F'] = 4,
    ['G'] = 4, ['H'] = 4, ['I'] = 4, ['J'] = 4, ['K'] = 4, ['L'] = 4,
    ['M'] = 4, ['N'] = 4, ['O'] = 4, ['P'] = 4, ['Q'] = 4, ['R'] = 4,
    ['S'] = 4, ['T'] = 4, ['U'] = 4, ['V'] = 4, ['W'] = 4, ['X'] = 4,
    ['Y'] = 4, ['Z'] = 4,

    [91 ... 96] = 0,

    ['a'] = 4, ['b'] = 4, ['c'] = 4, ['d'] = 4, ['e'] = 4, ['f'] = 4,
    ['g'] = 4, ['h'] = 4, ['i'] = 4, ['j'] = 4, ['k'] = 4, ['l'] = 4,
    ['m'] = 4, ['n'] = 4, ['o'] = 4, ['p'] = 4, ['q'] = 4, ['r'] = 4,
    ['s'] = 4, ['t'] = 4, ['u'] = 4, ['v'] = 4, ['w'] = 4, ['x'] = 4,
    ['y'] = 4, ['z'] = 4,
    
    ['"'] = 8, 
    ['_'] = 4,  
   

};

#define IS_WHITESPACE(c) (_charMap[(char)(c)] & 1)
#define IS_DIGIT(c) (_charMap[(char)(c)] & 2)
#define IS_ALPHA(c) (_charMap[(char)(c)] & 4)
#define IS_STR_DELIM(c) (_charMap[(char)(c)] & 8)

static inline void _lexerAdvanceAndRead(Lexer* lexer) {
    if (lexer -> position >= lexer -> len) {
        lexer -> character = '\0';
        return;
    }

    lexer -> position++;
    if (lexer -> position >= lexer -> len) {
        lexer -> character = '\0';
    } else {
        lexer -> character = lexer -> buffer[lexer -> position];

        if (lexer -> character == '\n') lexer -> line++;
    }
}

static inline void _lexerSkipWhitespace(Lexer* lexer) {
    while (IS_WHITESPACE(lexer -> character)) {
        if (lexer -> position >= lexer -> len) {
            return;
        }

        _lexerAdvanceAndRead(lexer);
    }
}

static void _lexerMapString(Lexer* lexer, char* string) {
    char* literal = malloc(strlen(string) + 1);
    strcpy(literal, string);

    for (int i = 0; i < _keywordCount; i++) {
        if (strcmp(literal, _keywordMap[i].key) == 0) {
            Token token = createToken(_keywordMap[i].type, literal, lexer -> line, true);
            pushTokenArray(lexer -> tokens, token);
            return;
        }
    }

    if (string[0] != '"') {
        Token token = createToken(TokenTypeIdentifier, literal, lexer -> line, true);
        pushTokenArray(lexer -> tokens, token);
        return;
    }

    Token token = createToken(TokenTypeString, literal, lexer -> line, true);
    pushTokenArray(lexer -> tokens, token);
}

static void _lexerMapNumber(Lexer* lexer, char* string) {
    char* literal = malloc(strlen(string) + 1);
    strcpy(literal, string);

    Token token = createToken(TokenTypeNumber, literal, lexer -> line, true);
    pushTokenArray(lexer -> tokens, token);
}

static bool _lexerMapNext(Lexer* lexer, char character) {
    char nextChar = lexer -> buffer[lexer -> position + 1]; 
    char* string = (char*) malloc(3);

    string[0] = character;
    string[1] = nextChar;
    string[2] = '\0';

    for (int i = 0; i < _operatorCount; i++) {
        if (strcmp(_operators[i].key, string) == 0) {
            Token token = createToken(_operators[i].type, string, lexer -> line, true);
            pushTokenArray(lexer -> tokens, token);

            _lexerAdvanceAndRead(lexer);
            return true;
        }
    }

    return false;
}

// Public

void lexerParse(Lexer* lexer) {
    while (lexer -> position < lexer -> len) {
        _lexerSkipWhitespace(lexer);

        switch (lexer -> character) {
            case '\0':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeEof, "EOF", lexer -> line, false));
                break;

            case '(':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeLeftParen, "(", lexer -> line, false));
                break;

            case ')':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeRightParen, ")", lexer -> line, false));
                break;

            case '{':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeLeftBrace, "{", lexer -> line, false));
                break;

            case '}':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeRightBrace, "}", lexer -> line, false));
                break;

            case ',':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeComma, ",", lexer -> line, false));
                break;

            case '.':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeDot, ".", lexer -> line, false));
                break;

            case '+':
                if (_lexerMapNext(lexer, '+')) break; 
                pushTokenArray(lexer -> tokens, createToken(TokenTypePlus, "+", lexer -> line, false));
                break;
                
            case '-':
                if (_lexerMapNext(lexer, '-')) break; 
                pushTokenArray(lexer -> tokens, createToken(TokenTypeMinus, "-", lexer -> line, false));
                break;

            case '*':
                if (_lexerMapNext(lexer, '*')) break; 
                pushTokenArray(lexer -> tokens, createToken(TokenTypeStar, "*", lexer -> line, false));
                break;

            case '/':
                if (_lexerMapNext(lexer, '/')) break; 
                pushTokenArray(lexer -> tokens, createToken(TokenTypeSlash, "/", lexer -> line, false));
                break;

            case '=':
                if (_lexerMapNext(lexer, '=')) break; 
                pushTokenArray(lexer -> tokens, createToken(TokenTypeEquals, "=", lexer -> line, false));
                break;

            case '!':
                if (_lexerMapNext(lexer, '!')) break; 
                pushTokenArray(lexer -> tokens, createToken(TokenTypeBang, "!", lexer -> line, false));
                break;

            case '<':
                if (_lexerMapNext(lexer, '<')) break; 
                pushTokenArray(lexer -> tokens, createToken(TokenTypeLess, "<", lexer -> line, false));
                break;

            case '>':
                if (_lexerMapNext(lexer, '>')) break; 
                pushTokenArray(lexer -> tokens, createToken(TokenTypeGreater, ">", lexer -> line, false));
                break;

            case ';':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeSemicolon, ";", lexer -> line, false));
                break;

            case '\'':
                pushTokenArray(lexer -> tokens, createToken(TokenTypeSingleQuote, "'", lexer -> line, false));
                break;

            default:
                break;
        }

        if (IS_ALPHA(lexer -> character)) {
            char string[1024];
            int i = 0;

            while (IS_ALPHA(lexer -> character)) {
                string[i++] = lexer -> character;

                _lexerAdvanceAndRead(lexer);
            }

            string[i] = '\0';
            _lexerMapString(lexer, string);
        } else if (IS_DIGIT(lexer -> character)) {
            char string[1024];
            int i = 0;

            while (IS_DIGIT(lexer -> character)) {
                string[i++] = lexer -> character;

                _lexerAdvanceAndRead(lexer);
            } 

            string[i] = '\0';

            _lexerMapNumber(lexer, string);
        } else {
            _lexerAdvanceAndRead(lexer);
        }
    }

    printTokenArray(lexer -> tokens);
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
    lexer -> character = lexer -> buffer[0];

    return lexer;
}

void freeLexer(Lexer** lexer) {
    if (!lexer || !*lexer) return;

    if ((*lexer) -> tokens) {
        freeTokenArray(&(*lexer) -> tokens);
    }

    free(*lexer);
    *lexer = NULL;
} 
