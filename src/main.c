#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


    return 0;
}
