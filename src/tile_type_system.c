#include <tile_type_system.h>
#include <stb_ds.h>

// static symtab_t symtab = NULL;

void symtab_add(symtab_t table, tile_symbol_t sym) {
    shput(table, "asd", sym);
}

void symtab_show(symtab_t table) {
    (void)table;
    printf("|");
    printf("%-20s", "key");
    printf("|");
    printf("%-20s", "value");
    printf("|");
    printf("\n");
}
