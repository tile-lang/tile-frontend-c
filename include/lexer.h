#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct
{
    size_t cursor;
    char current_char;
    const char* source_code;
    size_t source_code_size;
}tile_lexer_t;

// lexer_init needs const* char file_name
tile_lexer_t* lexer_init(char* src);

void tile_lexer_advance(tile_lexer_t* lexer);
void tile_lexer_skip_whitespace(tile_lexer_t* lexer);
// Peek function checks the next character without advancing
char tile_lexer_peek(tile_lexer_t* lexer);

// Function to match symbols
tile_token_t* tile_lexer_match_symbol(tile_lexer_t* lexer);
// Advances the lexer by a specific number of characters
tile_token_t* tile_lexer_get_next_token(tile_lexer_t* lexer);

tile_token_t* tile_lexer_collect_identifier(tile_lexer_t* lexer);
tile_token_t* tile_lexer_collect_number(tile_lexer_t* lexer);
tile_token_t* tile_lexer_collect_string(tile_lexer_t* lexer);

char* tile_lexer_get_current_char_as_string(tile_lexer_t* lexer);

int tile_lexer_is_at_end(tile_lexer_t* lexer);

#endif