#include "parser.h"
#include "../token/token.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline Token _parserPeek(Parser* parser);

static inline void _error(char* msg, Parser* parser) {
    size_t line = parser -> tokens -> tokens[parser -> current].line;
    printf("Error: %s on line %zu\n", msg, line);
    exit(1);
}

static inline bool _match(Parser* parser, TokenType type) {
    if (_parserPeek(parser).type == type) return true;
    return false;
}

static inline Token _parserPeek(Parser* parser) {
    if (parser -> current >= parser -> len) {
        return (Token) {TokenTypeEof, "Eof", parser -> current, false};
    }

    return parser -> tokens -> tokens[parser -> current];
}

static inline Token _parserAdvance(Parser* parser) {
    Token tok = _parserPeek(parser);

    if (parser -> current < parser -> len) {
        parser -> current++;
    }

    return tok;
}

void _pushDeclaration(AstNode* root, AstNode* node) {
    if (!root) return;

    if (root -> data.program.count >= root -> data.program.capacity) {
        root -> data.program.capacity *= 2;

        AstNode** newArr = realloc(root -> data.program.declarations, sizeof(AstNode) * root -> data.program.capacity);

        if (!newArr) {
            printf("realloc of decl failed\n");
            exit(1);
        }

        root -> data.program.declarations = newArr;
    }

    root -> data.program.declarations[root -> data.program.count] = node;
    root -> data.program.count++;
}

AstNode* _parseBody(Parser* parser) {
    return NULL;
}

AstNode* _parseFunction(Parser* parser) {
    AstNode* function = (AstNode*) malloc(sizeof(AstNode));
    function -> type = AstFn;

    if (!_match(parser, TokenTypeFn)) {
        _error("Expected 'fn'", parser);
    }

    _parserAdvance(parser);

    if (_parserPeek(parser).type != TokenTypeIdentifier) {
        _error("Expected function name", parser);
    }

    Token name = _parserAdvance(parser);

    if (!_match(parser, TokenTypeLeftParen)) {
        _error("Execepted '('", parser);
    }
    _parserAdvance(parser);

    if (!_match(parser, TokenTypeRightParen)) {
        _error("Expected ')'", parser);
    }
    _parserAdvance(parser);

    if (!_match(parser, TokenTypeLeftBrace)) {
        _error("Expected '{", parser);
    }
    _parserAdvance(parser);

    AstNode* body = _parseBody(parser);
    AstNode* func = malloc(sizeof(AstNode));

    func -> type = AstFn;
    func -> data.function.name = strdup(name.literal);
    func -> data.function.body = body;

    return function;
} 

AstNode* _parseProgram(Parser* parser) {
    AstNode* program = (AstNode*) malloc(sizeof(AstNode));
    program -> type = AstProgram;

    while (_parserPeek(parser).type != TokenTypeEof) {
        AstNode* function = _parseFunction(parser);
    }

    return program;
}

void parse(Parser* parser) {
    parser -> root = _parseProgram(parser);
}

Parser* createParser(TokenArray *arr, size_t len) {
    Parser* parser = (Parser*) malloc(sizeof(Parser));

    if (!parser) {
        return NULL;
    }

    parser -> tokens = arr;
    parser -> len = len;
    parser -> current = 0;

    AstNode* root = (AstNode*) malloc(sizeof(AstNode));

    if (!root) {
        free(parser);
        return NULL;
    }


    root -> type = AstProgram;
    root -> data.program.declarations = (AstNode**) malloc(sizeof(AstNode*) * 32);

    if (!root -> data.program.declarations) {
        free(root);
        free(parser);
        return NULL;
    }

    root -> data.program.count = 0;
    root -> data.program.capacity = 32;

    parser -> root = root;

    return parser; 
}

void freeParser(Parser** parser) {
    if (!parser || !*parser) return; 

    if ((*parser) -> root) {
        free((*parser) -> root -> data.program.declarations);
        free((*parser) -> root);
    }

    free(*parser);
    *parser = NULL;
}
