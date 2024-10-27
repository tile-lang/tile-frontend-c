#ifndef TILE_TYPE_SYSTEM_H
#define TILE_TYPE_SYSTEM_H

#include <tile_ast.h>
#include <stdbool.h>

typedef struct {
    bool is_used;
    tile_ast_t* ast_node;
} tile_symbol_t;

typedef struct {
    char* key;
    tile_symbol_t value;
} tile_symtable_t;

typedef tile_symtable_t* symtab_t;

void symtab_add(symtab_t table, tile_symbol_t sym);
void symtab_show(symtab_t table);

#endif//TILE_TYPE_SYSTEM_H