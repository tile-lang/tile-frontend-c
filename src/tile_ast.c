#include <tile_ast.h>

#define ARENA_IMPLEMENTATION

#include "common/arena.h"

#define ARENA_SIZE 1024

arena_t* ast_arena;

void tile_ast_arena_init() {
    ast_arena = arena_init(ARENA_SIZE);
}

void tile_ast_arena_destroy() {
    arena_destroy(ast_arena);
}

tile_ast_t* tile_ast_create(tile_ast_t ast) {
    tile_ast_t* ptr = arena_alloc(&ast_arena, sizeof(tile_ast_t));
    if (ptr)
        *ptr = ast;
    return ptr;
}

static void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

void tile_ast_show(tile_ast_t* node, int indent) {
    if (!node) return;

    print_indent(indent);
    switch (node->tag) {
        case AST_NONE:
            printf("NONE\n");
            break;
        case AST_PROGRAM:
            printf("PROGRAM\n");
            for (size_t i = 0; i < node->program.statement_count; i++) {
                tile_ast_show(node->program.statements[i], indent + 1);
            }
            break;
        
        // LITERALS
        case AST_LITERAL_INT:
            printf("LITERAL_INT\n");
            break;
        case AST_LITERAL_FLOAT:
            printf("LITERAL_FLOAT\n");
            break;
        case AST_LITERAL_STRING:
            printf("LITERAL_STRING\n");
            break;

        case AST_VARIABLE_DECL:
            printf("VARIABLE_DECL\n");
            break;
        case AST_VARIABLE_ASSIGN:
            printf("VARIABLE_ASSIGN\n");
            break;
        case AST_VARIABLE:
            printf("VARIABLE\n");
            break;

        case AST_BINARY_EXPR:
            printf("BINARY_DECL\n");
            break;
        case AST_UNARY_EXPR:
            printf("UNARY_DECL\n");
            break;

        case AST_WHILE_STATEMENT:
            printf("WhileStmt:\n");
            print_indent(indent + 1);
            printf("Condition:\n");
            tile_ast_show(node->while_statement.condition, indent + 2);
            print_indent(indent + 1);
            printf("Body:\n");
            tile_ast_show(node->while_statement.body, indent + 2);
            break;
    
        case AST_IF_STATEMENT:
            printf("IfStmt\n");
            print_indent(indent + 1);
            printf("Condition:\n");
            tile_ast_show(node->if_statement.condition, indent + 2);
            print_indent(indent + 1);
            printf("Body:\n");
            tile_ast_show(node->if_statement.body, indent + 2);
            print_indent(indent + 1);
            printf("Altarnate:\n");
            tile_ast_show(node->if_statement.altarnate, indent + 2);
            break;

        case AST_MATCH_STATEMENT:
            printf("MatchStmt\n");
            print_indent(indent + 1);
            printf("Expression:\n");
            tile_ast_show(node->match_statement.expression, indent + 2);
            print_indent(indent + 1);
            printf("Options:\n");
            
            if (node->match_statement.default_option != NULL) {
                // Match statement has a default option
                for(size_t i = 0; i < node->match_statement.option_count - 1; i++) {
                    tile_ast_show(node->match_statement.options[i], indent + 2);
                }
                print_indent(indent + 1);
                printf("Default:\n");
                tile_ast_show(node->match_statement.default_option, indent + 2);
                print_indent(indent + 1);
                break;
            }
            else {
                // Match statement hasn't a default option 
                for(size_t i = 0; i < node->match_statement.option_count; i++) {
                    tile_ast_show(node->match_statement.options[i], indent + 1);
                } 
            }
            break;
        case AST_OPTION_STATEMENT:
            printf("OptionStmt\n");
            print_indent(indent + 1);
            printf("Condition:\n");
            tile_ast_show(node->option_statement.condition, indent + 2);
            print_indent(indent + 1);
            printf("Statements:\n");
            for(size_t i = 0; i < node->option_statement.statement_count; i++) {
                tile_ast_show(node->option_statement.statements[i], indent + 2);
            }
            break;
        case AST_DEFAULT_STATEMENT:
            printf("DefaultStmt\n");
            print_indent(indent + 1);
            printf("Statements:\n");
            for(size_t i = 0; i < node->default_statement.statement_count; i++) {
                tile_ast_show(node->default_statement.statements[i], indent + 2);
            }
            break;

        case AST_FUNCTION_STATEMENT:
            printf("FuncStmt\n");
            print_indent(indent + 1);
            printf("Arguments:\n");
            for(size_t i = 0; i < node->function_statement.argument_count; i++) {
                tile_ast_show(node->function_statement.arguments[i], indent + 2);
            }
            tile_ast_show(node->function_statement.return_type, indent + 1);
            tile_ast_show(node->function_statement.body, indent + 2);
            break;
        case AST_FUNCTION_ARGUMENT:
            printf("Argument\n");
            break;
        case AST_FUNCTION_RETURN_TYPE:
            printf("Return type\n");
            break;
        case AST_RETURN_STATEMENT:
            printf("Return Statement\n");
            break;
        case AST_BLOCK:
            printf("BLOCK\n");
            for(size_t i = 0; i < node->block.statement_count; i++) {
                tile_ast_show(node->block.statements[i], indent + 1);
            }
            break;
        
        default:
            printf("UNKNOWN\n");
            break;
    }
}