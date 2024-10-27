#include <stdio.h>
#include <stdlib.h>
#include <tile_token.h>
#include <tile_parser.h>
// #include <tile_type_system.h>
#include <common/cmd_colors.h>
// #include <tile_cli_parser.h>
#define UNUSED(x) (void)(x)

arena_t* src_arena;

char* read_file_content(const char* file_name) {
    FILE* src_file = fopen(file_name, "r");

    if (src_file == NULL) {
        printf(CLR_RED"File can't be opened: "CLR_END"%s\n", file_name);
        fclose(src_file);
        exit(1);
    }

    fseek(src_file, 0L, SEEK_END);
    unsigned int file_size = ftell(src_file);
    fseek(src_file, 0L, SEEK_SET);

    if (file_size == 0) {
        printf(CLR_RED"File is empty: "CLR_END"%s\n", file_name);
        fclose(src_file);
        exit(1);
    }

    char* content = arena_alloc(&src_arena, file_size + 1);
    // printf("file size: %d\n", file_size);
    content[file_size] = '\0';

    char ch = 0;
    for (size_t i = 0; ch != EOF; i++) {
        ch = fgetc(src_file);
        content[i] = ch;
    }
    fclose(src_file);
    
    return content;
}

int main(int argc, char *argv[]) {
    // parse_args(argc, argv);  // Use the tile_cli_parser to handle arguments
    UNUSED(argc);
    // UNUSED(argv);

    src_arena = arena_init(4096);
    const char* const file_content = read_file_content(argv[1]);

    // printf("%s\n", file_content);

    tile_lexer_t lexer = tile_lexer_init(file_content, argv[1]);

    tile_token_t token = tile_token_create(TOKEN_NONE, NULL);
    tile_lexer_advance(&lexer);
    while(token.type != TOKEN_EOF) {
        token = tile_lexer_get_next_token(&lexer);
        printf("TOKEN(%d:%d: %d, %s)\n", lexer.loc.row, lexer.loc.col, token.type, token.value);
    }
    exit(EXIT_SUCCESS);


    tile_ast_arena_init();
    tile_parser_t parser = tile_parser_init(&lexer);
    tile_parser_eat(&parser, TOKEN_NONE);
    tile_ast_t* root = tile_parser_parse_statements(&parser);
    tile_ast_show(root, 0);

    // symtab_t symtab = NULL;
    // symtab_add(symtab, (tile_symbol_t) {
    //     .is_used = false,
    //     .ast_node = root,
    // });

    // symtab_show(symtab);
    

    arena_destroy(src_arena);
    tile_lexer_destroy(&lexer);
    tile_ast_arena_destroy();
    tile_ast_destroy(root);

    return 0;
}
