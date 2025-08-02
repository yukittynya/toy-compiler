#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/lexer/lexer.h"
#include "lib/parser/parser.h"

#define MAX_FILE_PATH 256

void handle_input(char* file_path, int argc, char** argv) {
    if (argc == 1) {
        printf("error: no file or cmd nya\n");
        exit(1);
    }

    if ((argc == 2) && (strcmp(argv[1], "help") == 0)) {
        printf("Usage: compiler path/to/file\n");
        exit(0);
    }

    strcpy(file_path, argv[1]);
}

int main(int argc, char *argv[]) {
    char file_path[MAX_FILE_PATH];
    handle_input(file_path, argc, argv);

    FILE* fptr = fopen(file_path, "r");

    fseek(fptr, 0, SEEK_END);
    int len = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    char buffer[len + 1];

    fread(buffer, 1, len, fptr);
    fclose(fptr);
    buffer[len] = '\0';

    Lexer* lexer = createLexer(buffer);

    lexerParse(lexer);

    Parser* parser = createParser(lexer -> tokens, lexer -> tokens -> count);
    parse(parser);

    freeParser(&parser);
    freeLexer(&lexer);
    
    return 0;
}
