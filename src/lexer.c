#include "../include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

tile_lexer_t* lexer_init(char* src){
    tile_lexer_t* lexer = (tile_lexer_t*)malloc(sizeof(tile_lexer_t));
    lexer->cursor = 0;
    lexer->current_char = src[lexer->cursor];
    lexer->source_code = src;
    lexer->source_code_size = strlen(src);
    return lexer;
}

void tile_lexer_advance(tile_lexer_t* lexer){
    if (!tile_lexer_is_at_end(lexer)){
        lexer->cursor++;
        lexer->current_char = lexer->source_code[lexer->cursor];
    }
}
void tile_lexer_skip_whitespace(tile_lexer_t* lexer){
    while (isspace(lexer->current_char)){
        tile_lexer_advance(lexer);
    }    
}
// Peek function checks the next character without advancing
char tile_lexer_peek(tile_lexer_t* lexer){
    if(tile_lexer_is_at_end(lexer))
        return '\0';
    return lexer->source_code[lexer->cursor + 1];
}

tile_token_t* tile_lexer_get_next_token(tile_lexer_t* lexer);

tile_token_t* tile_lexer_collect_identifier(tile_lexer_t* lexer);
tile_token_t* tile_lexer_collect_number(tile_lexer_t* lexer);
tile_token_t* tile_lexer_collect_string(tile_lexer_t* lexer);

char* tile_lexer_get_current_char_as_string(tile_lexer_t* lexer);

int tile_lexer_is_at_end(tile_lexer_t* lexer){
    return lexer->cursor >= lexer->source_code_size;
}