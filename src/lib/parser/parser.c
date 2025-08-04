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
    return (_parserPeek(parser).type == type) ? true : false;
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

static void _pushDeclaration(AstNode* root, AstNode* node) {
    if (!root) return;

    if (root -> data.program.count >= root -> data.program.capacity) {
        root -> data.program.capacity *= 2;

        AstNode** newArr = realloc(root -> data.program.declarations, sizeof(AstNode*) * root -> data.program.capacity);

        if (!newArr) {
            printf("realloc of decl failed\n");
            exit(1);
        }

        root -> data.program.declarations = newArr;
    }

    root -> data.program.declarations[root -> data.program.count] = node;
    root -> data.program.count++;
}

static AstNode* _parseLetStatement(Parser* parser) {
    AstNode* statement = malloc(sizeof(AstNode));
    statement -> type = AstAssignment;

    Token name = _parserAdvance(parser); 
    if (name.type != TokenTypeIdentifier) {
        _error("Expected identifier", parser);
    }

    statement -> data.assignment.name = strdup(name.literal);
    statement -> data.assignment.value = NULL;

    if (_parserPeek(parser).type == TokenTypeSemicolon) {
        _parserAdvance(parser);
        return statement;
    } else if (_parserPeek(parser).type == TokenTypeEquals) {
        _parserAdvance(parser);

        Token token = _parserPeek(parser); 
        
        if (token.type == TokenTypeString) {
            _parserAdvance(parser);
            AstNode* value = malloc(sizeof(AstNode));

            value -> type = AstString;
            value -> data.string = strdup(token.literal);

            statement -> data.assignment.value = value;
        } else if (token.type == TokenTypeNumber) {
            _parserAdvance(parser);
            AstNode* value = malloc(sizeof(AstNode));

            value -> type = AstNumber;
            value -> data.number = atof(token.literal);

            statement -> data.assignment.value = value;
        }
    } else {
        _error("Execepted assignment or ';'", parser);
    }

    if (!_match(parser, TokenTypeSemicolon)) {
        _error("Execepted ';'", parser);
    }

    _parserAdvance(parser);

    return statement;
}

static AstNode* _parsePrint(Parser* parser) {
    AstNode* call = malloc(sizeof(AstNode));
    
    call -> type = AstCall;
    call -> data.call.name = strdup("print");

    if (!_match(parser, TokenTypeLeftParen)) {
        _error("expected '('", parser);
    }
    _parserAdvance(parser);

    call -> data.call.argCount = 1;
    call -> data.call.args = malloc(sizeof(AstNode*) * call -> data.call.argCount);

    Token arg = _parserPeek(parser);
    if (arg.type == TokenTypeString) {
        _parserAdvance(parser);

        AstNode* argNode = malloc(sizeof(AstNode));
        argNode -> type = AstString;
        argNode -> data.string = strdup(arg.literal);

        call -> data.call.args[0] = argNode;
    } else if (arg.type == TokenTypeIdentifier) {
        _parserAdvance(parser);

        AstNode* argNode = malloc(sizeof(AstNode));
        argNode -> type = AstIdentifier;
        argNode -> data.identifier = strdup(arg.literal);

        call -> data.call.args[0] = argNode;
    } else if (arg.type == TokenTypeNumber) {
        _parserAdvance(parser);

        AstNode* argNode = malloc(sizeof(AstNode));
        argNode -> type = AstNumber;
        argNode -> data.number = atof(arg.literal);

        call -> data.call.args[0] = argNode;
    } else {
        _error("expected string, number or identifier in print statement", parser);
    }

    if (!_match(parser, TokenTypeRightParen)) {
        _error("Expected ')' after print argument", parser);
    }
    _parserAdvance(parser);
    
    if (!_match(parser, TokenTypeSemicolon)) {
        _error("Expected ';' after print", parser);
    }
    _parserAdvance(parser);

    return call;
}

static AstNode* _parseStatement(Parser* parser) {
    TokenType type = _parserPeek(parser).type;

    switch (type) {
        case TokenTypeLet:
            _parserAdvance(parser);
            return _parseLetStatement(parser);

        case TokenTypePrint:
            _parserAdvance(parser);
            return _parsePrint(parser);

        default:
            _error("unexpected token", parser);
            return NULL;
    }
}

static AstNode* _parseBlock(Parser* parser) {
    AstNode* block = malloc(sizeof(AstNode));

    block -> type = AstBlock;
    block -> data.block.count = 0;
    block -> data.block.capacity = 16;
    block -> data.block.statements = malloc(sizeof(AstNode*) * block -> data.block.capacity);

    while (_parserPeek(parser).type != TokenTypeRightBrace) {
        AstNode* statement = _parseStatement(parser);

        if (block -> data.block.count >= block -> data.block.capacity) {
            block -> data.block.capacity *= 2;
            block -> data.block.statements = realloc(block -> data.block.statements, sizeof(AstNode*) * block -> data.block.capacity);

            if (!block -> data.block.statements) {
                printf("Realloc failed of block -> data.block.statements\n");
                exit(1);
            }
        }

        block -> data.block.statements[block -> data.block.count++] = statement;
    }

    if (!_match(parser, TokenTypeRightBrace)) {
        _error("Expeced '}'", parser);
    }

    _parserAdvance(parser);

    return block;
}

static char** _parseParams(Parser* parser, size_t* paramCount) {
    size_t count = 0;
    size_t capacity = 32;
    char** params = malloc(sizeof(char*) * capacity);

    if (!params) {
        printf("malloc of params failed\n");
        exit(1);
    }

    if (_parserPeek(parser).type == TokenTypeRightParen) {
        *paramCount = 0;
        return params;
    }

    while (true) {
        if (_parserPeek(parser).type != TokenTypeIdentifier) {
            _error("Expected parameter", parser);
            break;
        }

        Token token = _parserAdvance(parser);

        if (count >= capacity) {
            capacity *= 2;
            params = realloc(params, capacity);

            if (!params) {
                printf("realloc of params failed\n");
                exit(1);
            }
        }

        params[count] = malloc(strlen(token.literal) + 1);
        strcpy(params[count++], token.literal); 

        if (_parserPeek(parser).type == TokenTypeRightParen) {
            break; 
        } else if (_parserPeek(parser).type == TokenTypeComma) {
            _parserAdvance(parser); 
        } else {
            _error("Expected ',' or ')' after parameter", parser);
            break;
        }
    }

    *paramCount = count;

    return params;
}

static AstNode* _parseFunction(Parser* parser) {
    AstNode* func = (AstNode*) malloc(sizeof(AstNode));
    func -> type = AstFn;

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

    size_t paramCount = 0;
    char** params = _parseParams(parser, &paramCount);

    if (!_match(parser, TokenTypeRightParen)) {
        _error("Expected ')'", parser);
    }
    _parserAdvance(parser);

    if (!_match(parser, TokenTypeLeftBrace)) {
        _error("Expected '{", parser);
    }
    _parserAdvance(parser);

    AstNode* block = _parseBlock(parser);

    func -> type = AstFn;
    func -> data.function.name = strdup(name.literal);
    func -> data.function.body = block;
    func -> data.function.params = params;
    func -> data.function.paramCount = paramCount;

    return func;
} 

AstNode* _parseProgram(Parser* parser) {
    AstNode* program = (AstNode*) malloc(sizeof(AstNode));

    program -> type = AstProgram;
    program -> data.program.count = 0;
    program -> data.program.capacity = 16;
    program -> data.program.declarations = malloc(sizeof(AstNode*) * program -> data.program.capacity);

    while (_parserPeek(parser).type != TokenTypeEof) {
        AstNode* function = _parseFunction(parser);
        _pushDeclaration(program, function);
    }

    return program;
}

void parse(Parser* parser) {
    parser -> root = _parseProgram(parser);
    printf("=== START AST ===\n");
    printAst(parser -> root, 0);
    printf("=== END AST ===\n");
}

void printAst(AstNode* node, int indent) {
    if (node == NULL) {
        printf("%*sNULL\n", indent, "");
        return;
    }
    
    printf("%*s", indent, "");
    
    switch (node->type) {
        case AstNumber:
            printf("NUMBER: %.2f\n", node -> data.number);
            break;
            
        case AstString:
            printf("STRING: \"%s\"\n", node -> data.string);
            break;
            
        case AstIdentifier:
            printf("IDENTIFIER: %s\n", node -> data.identifier);
            break;
            
        case AstBinaryOp:
            printf("BINARY_OP: '%c'\n", node -> data.binary.op);
            printf("%*sLeft:\n", indent + 2, "");
            printAst(node -> data.binary.left, indent + 4);
            printf("%*sRight:\n", indent + 2, "");
            printAst(node -> data.binary.right, indent + 4);
            break;
            
        case AstAssignment:
            printf("ASSIGNMENT: %s\n", node -> data.assignment.name);
            printf("%*sValue:\n", indent + 2, "");
            printAst(node -> data.assignment.value, indent + 4);
            break;
            
        case AstCall:
            printf("CALL: %s() [%zu args]\n", node -> data.call.name, node -> data.call.argCount);

            for (int i = 0; i < node -> data.call.argCount; i++) {
                printf("%*sArg %d:\n", indent + 2, "", i);
                printAst(node->data.call.args[i], indent + 4);
            }
            break;
            
        case AstReturn:
            printf("RETURN\n");

            if (node -> data.returnStatement.expression) {
                printf("%*sExpression:\n", indent + 2, "");
                printAst(node -> data.returnStatement.expression, indent + 4);
            }
            break;
            
        case AstBlock:
            printf("BLOCK [%zu statements]\n", node -> data.block.count);

            for (int i = 0; i < node -> data.block.count; i++) {
                printf("%*sStatement %d:\n", indent + 2, "", i);
                printAst(node->data.block.statements[i], indent + 4);
            }
            break;
            
        case AstFn:
            printf("FUNCTION: %s() [%zu params]\n", node -> data.function.name, node -> data.function.paramCount);
            
            if (node -> data.function.paramCount > 0) {
                printf("%*sParameters:\n", indent + 2, "");
                for (int i = 0; i < node -> data.function.paramCount; i++) {
                    printf("%*s%d: %s\n", indent + 4, "", i, node -> data.function.params[i]);
                }
            }
            
            printf("%*sBody:\n", indent + 2, "");
            printAst(node -> data.function.body, indent + 4);
            break;
            
        case AstProgram:
            printf("PROGRAM [%zu declarations]\n", node-> data.program.count);
            for (int i = 0; i < node -> data.program.count; i++) {
                printf("%*sDeclaration %d:\n", indent + 2, "", i);
                printAst(node -> data.program.declarations[i], indent + 4);
            }
            break;
            
        default:
            printf("UNKNOWN NODE TYPE: %d\n", node->type);
            break;
    }
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
