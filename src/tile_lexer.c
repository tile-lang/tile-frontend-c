#include "../include/tile_lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <../include/common/arena.h>

#define TOKENS_ARENA_CAPACITY 2048

tile_lexer_t tile_lexer_init(const char* src){
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

tile_token_t* tile_lexer_get_next_token(tile_lexer_t* lexer){ // needs strings
        tile_lexer_skip_whitespace(lexer);
        
        if (isalpha(lexer->current_char) || lexer->current_char == '_')
            return tile_lexer_collect_id(lexer);
        if (isdigit(lexer->current_char)) 
            return tile_lexer_collect_number(lexer);
        if (lexer->current_char == '"')
            return tile_lexer_collect_string(lexer);
        return tile_lexer_collect_one_chars(lexer);
}

tile_token_t* tile_lexer_collect_one_chars(tile_lexer_t* lexer){
    switch (lexer->current_char){
        case '(':
            tile_lexer_advance(lexer);
            return tile_token_create(TOKEN_LPAREN, '(');
        case ')':
            tile_lexer_advance(lexer);
            return tile_token_create(TOKEN_RPAREN, ')');
        case ';':
            tile_lexer_advance(lexer);
            return tile_token_create(TOKEN_SEMI, ';');
        default:
            return tile_token_create(TOKEN_NONE, NULL);    
    }
    return (void*)0;
}

tile_token_t* tile_lexer_collect_string(tile_lexer_t* lexer){

    tile_lexer_advance(lexer); // skip opening '"'

    char* buffer = NULL;
    size_t buffer_lenght = 0;

    while (lexer->current_char != '"' && !tile_lexer_is_at_end(lexer)){

        buffer_lenght++;
        buffer = (char*)arena_alloc(lexer->tokens_arena, buffer_lenght + 1);

        buffer[buffer_lenght - 1] = lexer->current_char;

        tile_lexer_advance(lexer);
    }

    tile_lexer_advance(lexer); // skip closing '"'

    return token_create(TOKEN_STRING, buffer);
}

tile_token_t* tile_lexer_collect_id(tile_lexer_t* lexer){
    char* buffer = NULL;
    size_t buffer_lenght = 0;

    while (isalnum(lexer->current_char) || lexer->current_char == '_'){

        buffer_lenght++;
        buffer = (char*)arena_alloc(lexer->tokens_arena, buffer_lenght + 1);

        buffer[buffer_lenght - 1] = lexer->current_char;

        tile_lexer_advance(lexer);
    }

    return token_create(TOKEN_ID, buffer);
}

tile_token_t* tile_lexer_collect_number(tile_lexer_t* lexer); // not implemented yet

char* tile_lexer_get_current_char_as_string(tile_lexer_t* lexer) {
    char* str = (char*)arena_alloc(lexer->tokens_arena, 2);
    str[0] = lexer->current_char;
    str[1] = '\0';
    return str;
}

int tile_lexer_is_at_end(tile_lexer_t* lexer){
    return lexer->cursor >= lexer->source_code_size;
}