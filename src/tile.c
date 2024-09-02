#include <stdio.h>
#include "../include/tile_lexer.h"

// char* read_file(const char* filename);
int main(int argc, char **argv) {
    (void)argc;
    (void)argv;    

    // char *source = read_file("../build/test.tile");

    tile_lexer_t lexer = tile_lexer_init(
        "\"bla1 bla2 3.2 bla4,5 -> ) <- this is fake parent\""
    );
    tile_token_t token;

    while(token.type != TOKEN_EOF){
        token = tile_lexer_get_next_token(&lexer);
        printf("TOKEN(%d, %s)\n", token.type, token.value);
    }
    return 0;
}


/* char* read_file(const char* filename){
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(length + 1);
    fread(content, 1, length, file);
    content[length] = '\0';

    fclose(file);
    return content;
} */
