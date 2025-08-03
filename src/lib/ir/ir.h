#ifndef IR_H
#define IR_H

#include "../parser/parser.h"
#include <stdlib.h>

typedef enum {
    IrLoadConst,
    IrStoreVar,
    IrLoadVar,
    IrCall
} IROpCode;

typedef struct {
    IROpCode op;
    union {
        double number;
        char* string;
        char* varName;
    } operand;
} IRInstruction;

typedef struct {
    IRInstruction* instructions;
    size_t count;
    size_t capacity;
} IR;

void generateIR(IR* ir, AstNode* root);
IR* createIR();
void freeIR(IR** ir);

#endif // !IR_H
