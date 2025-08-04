#ifndef IR_H
#define IR_H

#include "../parser/parser.h"
#include <stdlib.h>

typedef enum {
    IrFunction,
    IrCall,

    IrLet,
    IrLiteral,
    IrVariable
} IrType;

typedef struct IrNode {
    IrType type;
    union {
        struct {
            char* name;
            char** params;
            size_t paramCount;
            struct IrNode** body;
            size_t len;
        } function;

        struct {
            char* name;
            struct IrNode** args;
            size_t argCount;
        } call;

        struct {
            char* name;
            struct IrNode* value;
        } let;


        struct {
            double value;
        } literal;

        struct {
            char* name;
        } variable;
    } data;
} IrNode;

typedef struct {
    IrNode* instructions;
    size_t count;
    size_t capacity;
} IR;

void generateIR(IR* ir, AstNode* root);
IR* createIR();
void freeIR(IR** ir);

#endif // !IR_H
