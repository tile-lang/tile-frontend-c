#ifndef LEXER_H
#define LEXER_H

#include "tile_token.h"
#include "common/arena.h"

typedef struct {
    int row, col;
    const char* file_name;
} tile_loc_t;

typedef struct
{
    size_t cursor;
    char prev_char;
    char current_char;
    const char* source_code;
    size_t source_code_size;
    tile_loc_t loc;
    arena_t* tokens_arena;
}tile_lexer_t;

// lexer_init needs const* char file_name
tile_lexer_t tile_lexer_init(const char* src, const char* file_name);

void tile_lexer_advance(tile_lexer_t* lexer);
void tile_lexer_advance_by(tile_lexer_t* lexer, int steps);
void tile_lexer_skip_whitespace(tile_lexer_t* lexer);
// Peek function checks the next character without advancing
char tile_lexer_peek(tile_lexer_t* lexer);

tile_token_t tile_lexer_get_next_token(tile_lexer_t* lexer);

tile_token_t tile_lexer_collect_one_chars(tile_lexer_t* lexer);

tile_token_t tile_lexer_collect_id(tile_lexer_t* lexer);
tile_token_t tile_lexer_collect_number(tile_lexer_t* lexer);
tile_token_t tile_lexer_collect_string(tile_lexer_t* lexer);
tile_token_t tile_lexer_collect_symbol(tile_lexer_t* lexer);

char* tile_lexer_get_current_char_as_string(tile_lexer_t* lexer);

int tile_lexer_is_at_end(tile_lexer_t* lexer);

#endif