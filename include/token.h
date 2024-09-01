#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

typedef enum {
    TOKEN_EOF,
    // Literals
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_STRING,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_ID,

    //Keywords
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_MATCH,
    TOKEN_OPTION,

    // Symbols
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LSQUARE,
    TOKEN_RSQUARE,
    TOKEN_COMMA,

    TOKEN_DOT,
    TOKEN_COLON,
    TOKEN_SEMI,
    TOKEN_ASSIGN,
    TOKEN_COMMENT,
    TOKEN_QUESTION,

    TOKEN_LESS,
    TOKEN_LESSEQ,
    TOKEN_GREATER,
    TOKEN_GREATEREQ,
    TOKEN_EQ,
    TOKEN_NEQ,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_INC,
    TOKEN_DEC,
    TOKEN_MULTI,
    TOKEN_DIV,


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
