#include <tile_ast.h>

arena_t* ast_arena;

tile_ast_t* tile_ast_create(tile_ast_t ast) {
    tile_ast_t* ptr = arena_alloc(ast_arena, sizeof(tile_ast_t));
    if (ptr)
        *ptr = ast;
    return ptr;
}