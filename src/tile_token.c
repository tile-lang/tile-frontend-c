#include "../include/tile_token.h"
#include <stdlib.h>
#include <string.h>

tile_token_t* tile_token_create(token_type_t type, const char* value) {
    tile_token_t* token = (tile_token_t*)malloc(sizeof(tile_token_t));
    if (token == NULL) {
        return NULL;
    }

    token->type = type;

    token->value = (char*)malloc(strlen(value) + 1);
    if (token->value == NULL) {
        free(token);
        return NULL;
    }
    strcpy(token->value, value);

    return token;
}

void tile_token_free(tile_token_t* token) {
    if (token != NULL) {
        if (token->value != NULL) {
            free(token->value);
        }
        free(token);
    }
}

// Debug function (works with token_type_to_string)
void tile_token_print(tile_token_t* token) {
    if (token == NULL) return;

    const char* type_str = token_type_to_string(token->type);

    printf("Token Type: %s, Value: %s\n", type_str, token->value);
}

const char* tile_token_type_to_string(token_type_t type) {
    switch (type) {
        case TOKEN_TRUE:
            return "TRUE";
        case TOKEN_FALSE:
            return "FALSE";
        case TOKEN_STRING:
            return "STRING";
        case TOKEN_INT:
            return "INT";
        case TOKEN_FLOAT:
            return "FLOAT";
        case TOKEN_ID:
            return "IDENTIFIER";
        case TOKEN_LPAREN:
            return "LEFT PARENTHESIS";
        case TOKEN_RPAREN:
            return "RIGHT PARENTHESIS";
        case TOKEN_SEMI:
            return "SEMICOLON";
        default:
            return "UNKNOWN";
    }
}