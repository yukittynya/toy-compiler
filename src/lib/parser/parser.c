#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Token _parserPeek(Parser* parser) {
    if (parser -> current >= parser -> len) {
        return (Token) {TokenTypeEof, "Eof", parser -> current, false};
    }

    return parser -> tokens -> tokens[parser -> current];
}

Token _parserAdvance(Parser* parser) {
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

AstNode* parseTokens(Parser* parser) {
    AstNode* program = (AstNode*) malloc(sizeof(AstNode));
    program -> type = AstProgram;

    while (_parserPeek(parser).type != TokenTypeEof) {
        _parserAdvance(parser);
    }

    return program;
}

void parse(Parser* parser) {
    parser -> root = parseTokens(parser);
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
