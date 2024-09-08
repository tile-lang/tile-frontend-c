#include <stdio.h>
#include <tile_token.h>
#include <tile_lexer.h>
#include <tile_cli_parser.h>

int main(int argc, char *argv[]) {
    parse_args(argc, argv);  // Use the tile_cli_parser to handle arguments

    tile_lexer_t lexer = tile_lexer_init(
        "< <= [] ++ " , 
        NULL
    );
    tile_token_t token;
    
    while(token.type != TOKEN_EOF) {
        token = tile_lexer_get_next_token(&lexer);
        printf("TOKEN(%d, %s, Row %d, Col %d)\n", token.type, token.value, lexer.loc.row, lexer.loc.col);
    }
    return 0;
}

// char* read_file(const char* filename);
// char *source = read_file("../build/test.tile");
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
