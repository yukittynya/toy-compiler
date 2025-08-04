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

static void _pushInstruction(IR* ir, IrNode* node) {
    if (!ir || !ir -> instructions) return;

    if (ir -> count >= ir ->capacity) {
        ir -> capacity *= 2;

        ir -> instructions = realloc(ir -> instructions, sizeof(IrNode) * ir -> capacity);
        if (!ir -> instructions) {
            printf("_pushInstruction() realloc failed");
            exit(1);
        }
    }

    ir -> instructions[ir -> count++] = *node;
} 

IrNode* _generateCall(IR* ir, AstNode* root) {
    assert(root -> type == AstCall);
    IrNode* node = malloc(sizeof(IrNode));

    node -> type = IrCall;
    node -> data.call.name = strdup(root -> data.call.name);
    node -> data.call.argCount = root -> data.call.argCount;
    node -> data.call.args = malloc(sizeof(IrNode*) * root -> data.call.argCount);

    for (size_t i = 0; i < root -> data.call.argCount; i++) {
        AstNode* param = root -> data.call.args[i];
        IrNode* arg = malloc(sizeof(IrNode));

        if (param -> type == AstString) {
            arg -> type = IrStringLiteral; 
            arg -> data.stringLiteral.value = strdup(param -> data.string);
        } else if (param -> type == AstNumber) {
            arg -> type = IrNumberLiteral; 
            arg -> data.numberLiteral.value = param -> data.number;
        } else if (param -> type == AstIdentifier) {
            arg -> type = IrVariable;
            arg -> data.variable.name = strdup(param -> data.identifier);
        }

        node -> data.call.args[i] = arg;
    }

    return node;
}

IrNode* _generateAssignment(IR* ir, AstNode* root) {
    assert(root -> type == AstAssignment);
    IrNode* node = malloc(sizeof(IrNode));
    IrNode* value = malloc(sizeof(IrNode));

    node -> type = IrLet;
    node -> data.let.name = strdup(root -> data.assignment.name); 

    if (root -> data.assignment.value -> type == AstString) {
        value -> type = IrStringLiteral;
        value -> data.stringLiteral.value = strdup(root -> data.assignment.value -> data.string);
    } else if (root -> data.assignment.value -> type == AstNumber) {
        value -> type = IrNumberLiteral;
        value -> data.numberLiteral.value = root -> data.assignment.value -> data.number;
    }

    node -> data.let.value = value;

    return node;
}

IrNode* _generateStatement(IR* ir, AstNode* root) {
    IrNode* node = malloc(sizeof(IrNode));

    switch (root -> type) {
        case AstAssignment:
            node = _generateAssignment(ir, root);
            break;
        
        case AstCall:
            node = _generateCall(ir, root);
            break;
        default:
            break;
    }

    return node;
}

IrNode* _generateFn(IR* ir, AstNode* root) {
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
    node -> data.function.body = malloc(sizeof(IrNode*) * node -> data.function.len);
    for (size_t i = 0; i < node -> data.function.len; i++) {
        node -> data.function.body[i] = _generateStatement(ir, root -> data.function.body -> data.block.statements[i]);
    }

    return node;
}

void generateIR(IR* ir, AstNode* root) {
    if (!root) _error("generateIR(): root not found");

    switch (root -> type) {
        case AstAssignment: {
            IrNode* node = _generateAssignment(ir, root);
            _pushInstruction(ir, node);
            free(node);
            break;
        }

        case AstFn: {
            IrNode* node = _generateFn(ir, root);
            _pushInstruction(ir, node);
            free(node);
            break;
        }

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

void printIrNode(IrNode* node, int indent) {
    if (node == NULL) {
        printf("%*sNULL\n", indent, "");
        return;
    }
    
    printf("%*s", indent, "");
    
    switch (node->type) {
        case IrFunction:
            printf("IR_FUNCTION: %s() [%zu params]\n", 
                   node->data.function.name, 
                   node->data.function.paramCount);
            
            if (node->data.function.paramCount > 0) {
                printf("%*sParameters:\n", indent + 2, "");
                for (size_t i = 0; i < node->data.function.paramCount; i++) {
                    printf("%*s%zu: %s\n", indent + 4, "", i, node->data.function.params[i]);
                }
            }
            
            printf("%*sBody [%zu instructions]:\n", indent + 2, "", node->data.function.len);
            for (size_t i = 0; i < node->data.function.len; i++) {
                printf("%*sInstruction %zu:\n", indent + 4, "", i);
                printIrNode(node->data.function.body[i], indent + 6);
            }
            break;
            
        case IrCall:
            printf("IR_CALL: %s() [%zu args]\n", 
                   node->data.call.name, 
                   node->data.call.argCount);
            for (size_t i = 0; i < node->data.call.argCount; i++) {
                printf("%*sArg %zu:\n", indent + 2, "", i);
                printIrNode(node->data.call.args[i], indent + 4);
            }
            break;
            
        case IrLet:
            printf("IR_LET: %s\n", node->data.let.name);
            printf("%*sValue:\n", indent + 2, "");
            printIrNode(node->data.let.value, indent + 4);
            break;
            
        case IrStringLiteral:
            printf("IR_STRING: \"%s\"\n", node->data.stringLiteral.value);
            break;
            
        case IrNumberLiteral:
            printf("IR_NUMBER: %.2f\n", node->data.numberLiteral.value);
            break;
            
        case IrVariable:
            printf("IR_VARIABLE: %s\n", node->data.variable.name);
            break;
            
        default:
            printf("UNKNOWN IR NODE TYPE: %d\n", node->type);
            break;
    }
}

void printIR(IR* ir) {
    printf("=== START IR ===\n");
    printf("IR [%zu instructions]\n", ir->count);
    
    for (size_t i = 0; i < ir->count; i++) {
        printf("Instruction %zu:\n", i);
        printIrNode(&ir->instructions[i], 2);
    }
    
    printf("=== END IR ===\n");
}
