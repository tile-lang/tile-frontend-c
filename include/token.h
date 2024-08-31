#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

typedef enum {
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

    TOKEN_UNKNOWN  // For unknown or unrecognized tokens
} token_type_t;

typedef struct {
    token_type_t type;
    char* value;
} tile_token_t;

tile_token_t* token_create(token_type_t type, const char* value);

void token_free(tile_token_t* token);

void token_print(tile_token_t* token);

const char* token_type_to_string(token_type_t type);

#endif // TOKEN_H
