#include "ir.h"

#include "../parser/parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void _error(const char* msg) {
    printf("%s\n", msg);
    exit(1);
}

static void _pushInstruction(IR* ir, IrNode node) {
    if (!ir || !ir -> instructions) return;

    if (ir -> count >= ir ->capacity) {
        ir -> capacity *= 2;

        ir -> instructions = realloc(ir -> instructions, sizeof(IrNode) * ir -> capacity);
        if (!ir -> instructions) {
            printf("_pushInstruction() realloc failed");
            exit(1);
        }
    }

    ir -> instructions[ir -> count++] = node;
} 

void _generateAssignment(IR* ir, AstNode* root) {
    assert(root -> type == AstAssignment);
}

IrNode* _generateStatement(AstNode* root) {
    IrNode* node = malloc(sizeof(IrNode));

    return node;
}

void _generateFn(IR* ir, AstNode* root) {
    assert(root -> type == AstFn);

    IrNode* node = malloc(sizeof(IrNode));
    node -> type = IrFunction;
    node -> data.function.name = strdup(root -> data.function.name);
    node -> data.function.paramCount = root -> data.function.paramCount;

    node -> data.function.params = malloc(sizeof(char*) * node -> data.function.paramCount);
    for (size_t i = 0; i < node -> data.function.paramCount; i++) {
        node -> data.function.params[i] = strdup(root -> data.function.params[i]);
    }

    node -> data.function.len = root -> data.function.body -> data.block.count;
    for (size_t i = 0; i < node -> data.function.len; i++) {
        node -> data.function.body[i] = _generateStatement(root -> data.function.body -> data.block.statements[i]);
    }
}

void generateIR(IR* ir, AstNode* root) {
    if (!root) _error("generateIR(): root not found");

    switch (root -> type) {
        case AstAssignment:
            _generateAssignment(ir, root);
            break;

        case AstFn:
            _generateFn(ir, root);
            break;

        case AstProgram:
            for (int i = 0; i < root -> data.program.count; i++) {
                generateIR(ir, root -> data.program.declarations[i]);
            }
            break;

        default:
            return;
    }
}

IR* createIR() {
    IR* ir = malloc(sizeof(IR));

    if (!ir) {
        printf("IR malloc() failed\n");
        return NULL;
    }

    ir -> count = 0;
    ir -> capacity = 32;
    ir -> instructions = malloc(sizeof(IrNode*) * ir -> capacity);

    if (!ir -> instructions) {
        printf("IR Instructions malloc() failed\n");
        free(ir);
        return NULL;
    }

    return ir;
}

void _freeInstructions(IrNode** arr, size_t count) {
    if (count == 0) return;
    for (size_t i = 0; i < count; i++) {
        free(arr[i]);
    }
}

void freeIR(IR** ir) {
    if (!ir || !*ir) return;

    if ((*ir) -> instructions) {
        _freeInstructions(&(*ir) -> instructions, (*ir) -> count);
    }

    free((*ir));
    *ir = NULL;
}
