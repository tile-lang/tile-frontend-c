#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

typedef enum {
    TOKEN_NONE,

    // Literals
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_STRING,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_ID,

    // Symbols
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SEMI,

    TOKEN_UNKNOWN,  // For unknown or unrecognized tokens
    TOKEN_EOF,
} token_type_t;

typedef struct {
    token_type_t type;
    char* value;
} tile_token_t;

tile_token_t* tile_token_create(token_type_t type, const char* value);

void tile_token_free(tile_token_t* token);

void tile_token_print(tile_token_t* token);

const char* tile_token_type_to_string(token_type_t type);

#endif // TOKEN_H
