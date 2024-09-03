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

    // Keywords
    TOKEN_IF,
    TOKEN_ELSEIF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_MATCH,
    TOKEN_OPTION,
    TOKEN_BREAK,

    // Symbols
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,

    TOKEN_ASSIGN,
    TOKEN_COLON,
    TOKEN_SEMI,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULT,
    TOKEN_DIV,

    TOKEN_UNKNOWN,  // For unknown or unrecognized tokens
    TOKEN_EOF,
} token_type_t;

typedef struct {
    token_type_t type;
    char* value;
} tile_token_t;

tile_token_t tile_token_create(token_type_t type, char* value);

#endif // TOKEN_H