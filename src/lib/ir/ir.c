#include "ir.h"

#include "../parser/parser.h"

#include <stdlib.h>
#include <stdio.h>

void _error(const char* msg) {
    printf("%s\n", msg);
    exit(1);
}

void generateIR(IR* ir, AstNode* root) {
    if (!root) _error("generateIR(): root not found");

    switch (root -> type) {
        case AstFn:
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
    ir -> instructions = malloc(sizeof(IRInstruction*) * ir -> capacity);

    if (!ir -> instructions) {
        printf("IR Instructions malloc() failed\n");
        free(ir);
        return NULL;
    }

    return ir;
}

void _freeInstructions(IRInstruction** arr, size_t count) {
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
