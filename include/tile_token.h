#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

typedef enum {
    TOKEN_NONE,

    // Literals
    TOKEN_TRUE_LITERAL,
    TOKEN_FALSE_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,

    TOKEN_ID,

    // Keywords
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_MATCH,
    TOKEN_OPTION,
    TOKEN_DEFAULT,
    TOKEN_BREAK,
    //TOKEN_FUNC,
    TOKEN_RETURN,

    // Variable Keywords
    TOKEN_TYPE_KW,

    // Symbols
    TOKEN_LESS,
    TOKEN_LESSEQ,
    TOKEN_GREATER,
    TOKEN_GREATEREQ,
    TOKEN_EQ,
    TOKEN_NEQ,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LSQUARE,
    TOKEN_RSQUARE,

    TOKEN_ASSIGN,
    TOKEN_COLON,
    TOKEN_SEMI,
    TOKEN_COMMA,
    TOKEN_COMMENT,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,

    TOKEN_INC,
    TOKEN_DEC,
    
    TOKEN_UNKNOWN,  // For unknown or unrecognized tokens
    TOKEN_EOF,
} token_type_t;

typedef struct {
    size_t row, col;
    const char* file_name;
} tile_loc_t;

typedef struct {
    token_type_t type;
    char* value;
    tile_loc_t loc;
} tile_token_t;

tile_token_t tile_token_create(token_type_t type, char* value);

#endif // TOKEN_H