#ifndef TILE_AST_H
#define TILE_AST_H

#include <tile_lexer.h>

typedef struct tile_ast {
    enum {
        AST_NONE,
        AST_PROGRAM, // root node

        AST_LITERAL_INT,
        AST_LITERAL_FLOAT,
        AST_LITERAL_STRING,
        
        AST_VARIABLE_DECL,
        AST_VARIABLE_ASSIGN,
        AST_VARIABLE,

        AST_BINARY_EXPR,
        AST_UNARY_EXPR,

        AST_WHILE_STATEMENT,
        AST_IF_STATEMENT,
        AST_MATCH_STATEMENT,
        AST_OPTION_STATEMENT,
        AST_DEFAULT_STATEMENT,

        AST_BLOCK,
    } tag;

    tile_loc_t loc;

    union {
        // AST_PROGRAM
        struct ast_program {
            struct tile_ast** statements;
            size_t statement_count;
        } program;

        // AST_STRING
        struct ast_string {
            const char* text_value;
            const char* string_value;  
        } string;

        // AST_LITERAL_INT and AST_LITERAL_FLOAT
        struct ast_number {
            const char* text_value;
            union {
                int value;
                float fvalue;
            }; 
        } number;

        // AST_VARIABLE (declaration, assignment, and usage)
        struct ast_variable_decl {
            const char* type;
            const char* name;
        } variable_decl;

        struct ast_variable_assign {
            const char* type;
            const char* name;
            struct tile_ast* value;
        } variable_assign;

        struct ast_variable {
            const char* name;
        } variable;

        // AST_BINARY_EXPR
        struct ast_binary_expr {
            const char* op; // + - / *
            struct tile_ast* left;
            struct tile_ast* right;
        } binary_expr;

        // AST_UNARY_EXPR
        struct ast_unary_expr {
            const char* op; // negative (-5) or logical not (!x)
            struct tile_ast* operand;
        } unary_expr;

        // AST_WHILE
        struct ast_while_statement {
            struct tile_ast* condition;
            struct tile_ast* body;
        } while_statement;

        // AST_IF_STATEMENT
        struct ast_if_statement {
            struct tile_ast* condition;
            struct tile_ast* body;
            struct tile_ast* altarnate; // Optional, can be NULL
        } if_statement;

        struct ast_match_statement {
            struct tile_ast* expression;
            struct tile_ast** options;
            struct tile_ast* default_option;
            size_t option_count;
        } match_statement;

        struct ast_option_statement {
            struct tile_ast* condition;
            struct tile_ast** statements;
            size_t statement_count;
        } option_statement;

        struct ast_default_statement {
            struct tile_ast** statements;
            size_t statement_count;
        } default_statement;

        // AST_BLOCK, body parts of functions, if elses '{ }'
        struct ast_block {
            struct tile_ast** statements;
            size_t statement_count;
        } block;
    };

} tile_ast_t;

void tile_ast_arena_init();
void tile_ast_arena_destroy();

tile_ast_t* tile_ast_create(tile_ast_t ast);

void tile_ast_show(tile_ast_t* node, int indent);

#endif // TILE_AST_H
