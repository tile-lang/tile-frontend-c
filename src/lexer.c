#include "../include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


tile_lexer_t* lexer_init(char* src){
    tile_lexer_t* lexer = (tile_lexer_t*)malloc(sizeof(tile_lexer_t));
    lexer->cursor = 0;
    lexer->current_char = src[lexer->cursor];
    lexer->source_code = src;
    lexer->source_code_size = strlen(src);
    return lexer;
}

void tile_lexer_advance(tile_lexer_t* lexer){
    if (!tile_lexer_is_at_end(lexer)){
        lexer->cursor++;
        lexer->current_char = lexer->source_code[lexer->cursor];
    }
}

void tile_lexer_advance_by(tile_lexer_t* lexer, int steps) {
    for (int i = 0; i < steps; i++) {
        tile_lexer_advance(lexer);
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

tile_token_t* tile_lexer_get_next_token(tile_lexer_t* lexer);

tile_token_t* tile_lexer_collect_identifier(tile_lexer_t* lexer);
tile_token_t* tile_lexer_collect_number(tile_lexer_t* lexer);
tile_token_t* tile_lexer_collect_string(tile_lexer_t* lexer);

char* tile_lexer_get_current_char_as_string(tile_lexer_t* lexer);

int tile_lexer_is_at_end(tile_lexer_t* lexer){
    return lexer->cursor >= lexer->source_code_size;
}

typedef struct {
    const char* text;
    token_type_t type;
} tile_lextoken_t;

#define MAX_SYMBOL_LENGTH 2

static const tile_lextoken_t symbols[] = {

    { "<", TOKEN_LESS },
    { "<=", TOKEN_LESSEQ },
    { ">", TOKEN_GREATER },
    { ">=", TOKEN_GREATEREQ },
    { "==", TOKEN_EQ },
    { "!=", TOKEN_NEQ },

    { "+", TOKEN_PLUS },
    { "-", TOKEN_MINUS },
    { "++", TOKEN_INC },
    { "--", TOKEN_DEC },
    { "*", TOKEN_MULTI },
    { "/", TOKEN_DIV },

    { "{", TOKEN_LBRACE },
    { "}", TOKEN_RBRACE },
    { "(", TOKEN_LPAREN },
    { ")", TOKEN_RPAREN },
    { "[", TOKEN_LSQUARE },
    { "]", TOKEN_RSQUARE },
    { ",", TOKEN_COMMA },

    { ".", TOKEN_DOT },
    { ":", TOKEN_COLON },
    { ";", TOKEN_SEMI },
    { "=", TOKEN_ASSIGN },
    { "//", TOKEN_COMMENT },
    { "?", TOKEN_QUESTION },

    { NULL,  }
};

static const tile_lextoken_t keywords[] = {
    { "if", TOKEN_IF },
    { "else", TOKEN_ELSE },
    { "for", TOKEN_FOR },
    { "while", TOKEN_WHILE },
    { "match", TOKEN_MATCH },
    { "option", TOKEN_OPTION },

    { NULL,  }
};

tile_token_t* tile_lexer_match_symbol(tile_lexer_t* lexer) {
    for (int i = 0; symbols[i].text != NULL; i++) {
        int len = strlen(symbols[i].text);
        if (strncmp(&lexer->source_code[lexer->cursor], symbols[i].text, len) == 0) {
            tile_lexer_advance_by(lexer, len);  // Move the cursor forward
            return token_create(symbols[i].type, symbols[i].text);  // Create token with the symbol
        }
    }
    return NULL; // No matching symbol found
}

tile_token_t* tile_lexer_get_next_token(tile_lexer_t* lexer) {
    while (lexer->current_char != '\0') {
        if (isspace(lexer->current_char)) {
            tile_lexer_skip_whitespace(lexer);
            continue;
        }

        // Check if the current character starts a symbol
        tile_token_t* symbol_token = tile_lexer_match_symbol(lexer);
        if (symbol_token != NULL) {
            return symbol_token;
        }

        // Handle identifiers
        if (isalpha(lexer->current_char)) {
            return tile_lexer_collect_identifier(lexer);
        }

        // Handle numbers
        if (isdigit(lexer->current_char)) {
            return tile_lexer_collect_number(lexer);
        }

        // Handle unexpected characters
        lexer->cursor++;
        lexer->current_char = lexer->source_code[lexer->cursor];
    }

    return token_create(TOKEN_EOF, "");  // Return EOF token at the end of input
}

tile_token_t* tile_lexer_collect_identifier(tile_lexer_t* lexer) {
    size_t start = lexer->cursor;

    while (isalnum(lexer->current_char) || lexer->current_char == '_') {
        tile_lexer_advance(lexer);
    }

    size_t length = lexer->cursor - start;
    char* identifier = (char*)malloc(length + 1);
    strncpy(identifier, lexer->source_code + start, length);
    identifier[length] = '\0';
    
    // Check if the identifier is a keyword
    for (int i = 0; keywords[i].text != NULL; i++) {
        if (strcmp(identifier, keywords[i].text) == 0) {
            free(identifier);
            return token_create(keywords[i].type, keywords[i].text);
        }
    }

    // Create the token
    tile_token_t* token = token_create(TOKEN_ID, identifier);
    free(identifier);  // Free the temporary string

    return token;
}
