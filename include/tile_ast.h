#ifndef TILE_AST_H
#define TILE_AST_H

#include <tile_lexer.h>

typedef enum {
    OP_ADD,   // +
    OP_SUB,   // -
    OP_MUL,   // *
    OP_DIV    // /
} binary_operator_t;

typedef enum {
    PRIM_TYPE_NONE,
    
    PRIM_TYPE_INT,
    PRIM_TYPE_FLOAT,
} primitive_type;

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

        AST_FUNCTION_STATEMENT,
        AST_FUNCTION_ARGUMENT,
        AST_FUNCTION_RETURN_TYPE,
        AST_RETURN_STATEMENT,

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
            primitive_type type;
            const char* name;
        } variable_decl;

        struct ast_variable_assign {
            const char* name;
            struct tile_ast* value;
        } variable_assign;

        struct ast_variable {
            const char* name;
        } variable;

        // AST_BINARY_EXPR
        struct ast_binary_expr {
            binary_operator_t op;
            struct tile_ast* left;
            struct tile_ast* right;
        } binary_expr;

        // AST_UNARY_EXPR
        struct ast_unary_expr {
            const char* op; // negative (-5) or logical not (!x)
            struct tile_ast* operand;
        } unary_expr;

        // AST_WHILE_STATEMENT
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

        // AST_MATCH_STATEMENT
        struct ast_match_statement {
            struct tile_ast* expression;
            struct tile_ast** options;
            struct tile_ast* default_option;
            size_t option_count;
        } match_statement;

        // AST_OPTION_STATEMENT
        struct ast_option_statement {
            struct tile_ast* condition;
            struct tile_ast** statements;
            size_t statement_count;
        } option_statement;

        // AST_DEFAULT_STATEMENT
        struct ast_default_statement {
            struct tile_ast** statements;
            size_t statement_count;
        } default_statement;

        // AST_FUNCTION_STATEMENT
        struct ast_function_statement {
            struct tile_ast* return_type;
            const char* func_name;
            struct tile_ast** arguments;
            size_t argument_count;
            struct tile_ast* body;
        } function_statement;

        // AST_FUNCTION_ARGUMENT
        struct ast_argument {
            primitive_type type_name;
            const char* var_name;
        } argument;

        // AST_FUNCTION_RETURN_TYPE
        struct ast_return_type {
            primitive_type type_name;
        } return_type;

        // AST_RETURN_STATEMENT
        struct ast_return_statement {
            struct tile_ast* expression;
        } return_statement;

        // AST_BLOCK
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