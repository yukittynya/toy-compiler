#ifndef PARSER_H
#define PARSER_H

#include "../token/token.h"

typedef enum {
    AstNumber,
    AstString,
    AstIdentifier,

    AstBinaryOp,
    AstAssignment,
    AstCall,

    AstReturn,
    AstBlock,

    AstFn,
    AstProgram
} AstNodeType;

typedef struct AstNode {
    AstNodeType type;
    union {
        double number;
        char* string;
        char* identifier;

        struct {
            struct AstNode* left;
            struct AstNode* right;
            char op;
        } binary;

        struct {
            char* name;
            struct AstNode* value;
        } assignment;

        struct {
            char* name;
            struct AstNode** args;
            size_t argCount;
        } call;

        struct {
            struct AstNode* expression;
        } returnStatement;

        struct {
            struct AstNode** statements;
            size_t count;
            size_t capacity;
        } block;

        struct {
            char* name;
            char** params;
            size_t paramCount;
            struct AstNode* body;
        } function;

        struct {
            struct AstNode** declarations;
            size_t count;
            size_t capacity;
        } program;
    } data;
} AstNode;

typedef struct {
    TokenArray* tokens;
    size_t len;
    size_t current;
    AstNode* root;
} Parser;

Parser* createParser(TokenArray* arr, size_t len);
void freeParser(Parser** parser);
void parse(Parser* parser);
void printAst(AstNode* root, int indent);

#endif // !define PARSER_H
