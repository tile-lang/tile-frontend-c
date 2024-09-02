#include "../include/tile_token.h"
#include <stdlib.h>
#include <string.h>

tile_token_t* token_create(token_type_t type, const char* value) {
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

void token_free(tile_token_t* token) {
    if (token != NULL) {
        if (token->value != NULL) {
            free(token->value);
        }
        free(token);
    }
}

// Debug function (works with token_type_to_string)
void token_print(tile_token_t* token) {
    if (token == NULL) return;

    const char* type_str = token_type_to_string(token->type);

    printf("Token Type: %s, Value: %s\n", type_str, token->value);
}

const char* token_type_to_string(token_type_t type) {
    switch (type) {
        // Literals
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
        case TOKEN_EOF:
            return "EOF";

        // Keywords
        case TOKEN_IF:
            return "IF";
        case TOKEN_ELSE:
            return "ELSE";
        case TOKEN_FOR:
            return "FOR";
        case TOKEN_WHILE:
            return "WHILE";
        case TOKEN_MATCH:
            return "MATCH";
        case TOKEN_OPTION:
            return "OPTION";

        // Symbols
        case TOKEN_LPAREN:
            return "LEFT PARENTHESIS";
        case TOKEN_RPAREN:
            return "RIGHT PARENTHESIS";
        case TOKEN_LBRACE:
            return "LEFT BRACE";
        case TOKEN_RBRACE:
            return "RIGHT BRACE";
        case TOKEN_LSQUARE:
            return "LEFT SQUARE BRACKET";
        case TOKEN_RSQUARE:
            return "RIGHT SQUARE BRACKET";
        case TOKEN_COMMA:
            return "COMMA";
        case TOKEN_DOT:
            return "DOT";
        case TOKEN_COLON:
            return "COLON";
        case TOKEN_SEMI:
            return "SEMICOLON";
        case TOKEN_ASSIGN:
            return "ASSIGNMENT";
        case TOKEN_COMMENT:
            return "COMMENT";
        case TOKEN_QUESTION:
            return "QUESTION MARK";

        // Comparison Operators
        case TOKEN_LESS:
            return "LESS THAN";
        case TOKEN_LESSEQ:
            return "LESS THAN OR EQUAL";
        case TOKEN_GREATER:
            return "GREATER THAN";
        case TOKEN_GREATEREQ:
            return "GREATER THAN OR EQUAL";
        case TOKEN_EQ:
            return "EQUAL";
        case TOKEN_NEQ:
            return "NOT EQUAL";

        // Arithmetic Operators
        case TOKEN_PLUS:
            return "PLUS";
        case TOKEN_MINUS:
            return "MINUS";
        case TOKEN_INC:
            return "INCREMENT";
        case TOKEN_DEC:
            return "DECREMENT";
        case TOKEN_MULTI:
            return "MULTIPLICATION";
        case TOKEN_DIV:
            return "DIVISION";


        default:
            return "UNKNOWN";
    }
}