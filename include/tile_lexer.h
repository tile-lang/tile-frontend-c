#ifndef LEXER_H
#define LEXER_H

#include "tile_token.h"
#include "common/arena.h"
#include <stdbool.h>

typedef struct {
    size_t cursor;
    char prev_char;
    char current_char;
    char next_char;
    const char* source_code;
    size_t* line_lengths;
    size_t source_code_size;
    tile_loc_t loc;
    arena_t* arena;
} tile_lexer_t;

// lexer_init needs const* char file_name
tile_lexer_t tile_lexer_init(const char* src, const char* file_name);
void tile_lexer_destroy(tile_lexer_t* lexer);

void tile_lexer_advance(tile_lexer_t* lexer);
void tile_lexer_advance_by(tile_lexer_t* lexer, size_t steps);
void tile_lexer_skip_whitespace(tile_lexer_t* lexer);
void tile_lexer_skip_line(tile_lexer_t* lexer);
// Peek function checks the next character without advancing
// if you call 2 times it peeks 2 character ahead
char tile_lexer_peek(tile_lexer_t* lexer);
// Peek reset function resets the peek function
void tile_lexer_peek_reset(tile_lexer_t *lexer);
// Peek until endline function peeks until see the given character and returns distance (it automaticly resets peek at the end)
size_t tile_lexer_peek_until_endline(tile_lexer_t* lexer, char* character);

const char* tile_lexer_get_line(tile_lexer_t *lexer, size_t row, size_t* character_count);

tile_token_t tile_lexer_get_next_token(tile_lexer_t* lexer);

tile_token_t tile_lexer_collect_id(tile_lexer_t* lexer);
tile_token_t tile_lexer_collect_number(tile_lexer_t* lexer);
tile_token_t tile_lexer_collect_string(tile_lexer_t* lexer);
tile_token_t tile_lexer_collect_symbol(tile_lexer_t* lexer);

int tile_lexer_is_at_end(tile_lexer_t* lexer);

#endif