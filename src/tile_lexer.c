#define ARENA_IMPLEMENTATION
#include "../include/tile_lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <../include/common/arena.h>

#define TOKENS_ARENA_CAPACITY 2048

tile_lexer_t tile_lexer_init(const char* src) {
    tile_lexer_t lexer = {
        .cursor = 0,
        .prev_char = src[0],
        .current_char = src[0],
        .source_code = src,
        .source_code_size = strlen(src),
        .tokens_arena = arena_init(TOKENS_ARENA_CAPACITY),
    };
    return lexer;
    // tile_lexer_t* lexer = (tile_lexer_t*)malloc(sizeof(tile_lexer_t));
}

void tile_lexer_advance(tile_lexer_t* lexer) {
    if (!tile_lexer_is_at_end(lexer)) {
        lexer->cursor++;
        lexer->current_char = lexer->source_code[lexer->cursor];
    }
}

void tile_lexer_skip_whitespace(tile_lexer_t* lexer) {
    while (isspace(lexer->current_char)) {
        tile_lexer_advance(lexer);
    }    
}

// Peek function checks the next character without advancing
char tile_lexer_peek(tile_lexer_t* lexer) {
    if(tile_lexer_is_at_end(lexer))
        return '\0';
    return lexer->source_code[lexer->cursor + 1];
}

tile_token_t tile_lexer_get_next_token(tile_lexer_t* lexer) { // needs strings
        tile_lexer_skip_whitespace(lexer);
        
        if (tile_lexer_is_at_end(lexer))
            return tile_token_create(TOKEN_EOF, "eof");
        if (isalpha(lexer->current_char) || lexer->current_char == '_')
            return tile_lexer_collect_id(lexer);
        if (isdigit(lexer->current_char)) 
            return tile_lexer_collect_number(lexer);
        if (lexer->current_char == '"')
            return tile_lexer_collect_string(lexer);
        return tile_lexer_collect_one_chars(lexer);
}

tile_token_t tile_lexer_collect_one_chars(tile_lexer_t* lexer) {
    switch (lexer->current_char){
        case '(':
            tile_lexer_advance(lexer);
            return tile_token_create(TOKEN_LPAREN, "(");
        case ')':
            tile_lexer_advance(lexer);
            return tile_token_create(TOKEN_RPAREN, ")");
        case ';':
            tile_lexer_advance(lexer);
            return tile_token_create(TOKEN_SEMI, ";");
        default:
            return tile_token_create(TOKEN_NONE, NULL);
    }
    /* return (void)0; */
}

tile_token_t tile_lexer_collect_string(tile_lexer_t* lexer) {

    tile_lexer_advance(lexer); // skip opening '"'

    size_t len = 0;
    char temp_val[128];
    while (lexer->current_char != '"') {
        temp_val[len] = lexer->current_char;
        len++;
        tile_lexer_advance(lexer);
    }
    temp_val[len] = '\0';
    len++;
    char* val = (char*)arena_alloc(lexer->tokens_arena, len);
    memmove(val, temp_val, len);
    tile_token_t token = tile_token_create(TOKEN_STRING, val);
    
    tile_lexer_advance(lexer); // skip closing '"'

    return token;
}

tile_token_t tile_lexer_collect_id(tile_lexer_t *lexer) {
    size_t len = 0;
    char temp_val[128];
    while (isalnum(lexer->current_char) || lexer->current_char == '_') {
        temp_val[len] = lexer->current_char;
        len++;
        tile_lexer_advance(lexer);
    }
    temp_val[len] = '\0';
    len++;
    char* val = (char*)arena_alloc(lexer->tokens_arena, len);
    memmove(val, temp_val, len);
    tile_token_t token = tile_token_create(TOKEN_ID, val);
    return token;
}

tile_token_t tile_lexer_collect_number(tile_lexer_t* lexer) {
    size_t len = 0;
    char temp_val[128];
    token_type_t type = TOKEN_FLOAT;

    while(isdigit(lexer->current_char)) {
        temp_val[len] = lexer->current_char;
        len++;
        tile_lexer_advance(lexer);
    }

    if (lexer->current_char == '.') {
        temp_val[len] = lexer->current_char;
        len++;
        tile_lexer_advance(lexer);

        while (isdigit(lexer->current_char)) {
            temp_val[len] = lexer->current_char;
            len++;
            tile_lexer_advance(lexer);
        }   
    }
    else {
        type = TOKEN_INT;
    }
    temp_val[len] = '\0';
    len++;
    char* val = (char*)arena_alloc(lexer->tokens_arena, len);
    memmove(val, temp_val, len);
    tile_token_t token = tile_token_create(type, val);
    return token;
}

char* tile_lexer_get_current_char_as_string(tile_lexer_t* lexer) {
    char* str = (char*)arena_alloc(lexer->tokens_arena, 2);
    str[0] = lexer->current_char;
    str[1] = '\0';
    return str;
}

int tile_lexer_is_at_end(tile_lexer_t* lexer) {
    return lexer->cursor >= lexer->source_code_size;
}