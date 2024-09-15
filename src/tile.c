#include <stdio.h>
#include <tile_token.h>
#include <tile_parser.h>
// #include <tile_cli_parser.h>
#define UNUSED(x) (void)(x)

int main(int argc, char *argv[]) {
    // parse_args(argc, argv);  // Use the tile_cli_parser to handle arguments
    UNUSED(argc);
    UNUSED(argv);

    tile_lexer_t lexer = tile_lexer_init(
        "match (5) { option 1: int x; option 2: int y = 2; option 3: int z; default: float x = 3.5; }"
        , NULL
    );

    tile_ast_arena_init();
    tile_parser_t parser = tile_parser_init(&lexer);
    tile_parser_eat(&parser, TOKEN_NONE);
    tile_ast_t* root = tile_parser_parse_statements(&parser);
    tile_ast_show(root, 0);
    
    // tile_token_t token;
    
    // while(token.type != TOKEN_EOF) {
    //     token = tile_lexer_get_next_token(&lexer);
    //     printf("TOKEN(%d, %s, Row %d, Col %d)\n", token.type, token.value, lexer.loc.row, lexer.loc.col);
    // }

    tile_ast_arena_destroy();

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
