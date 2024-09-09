#ifndef TILE_PARSER_H_
#define TILE_PARSER_H_

#include <tile_lexer.h>
#include <tile_ast.h>

typedef struct {

    tile_token_t current_token;
    tile_lexer_t* lexer;

}tile_parser_t;

tile_parser_t tile_parser_init(tile_lexer_t* lexer);

void tile_parser_eat(tile_parser_t* parser, int token_type);

tile_ast_t* tile_parser_parse_expression(tile_parser_t* parser);

tile_ast_t* tile_parser_parse_statement(tile_parser_t* parser);

tile_ast_t* tile_parser_parse_block(tile_parser_t* parser);
#endif