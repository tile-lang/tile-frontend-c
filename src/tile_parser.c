#include <tile_parser.h>

tile_parser_t tile_parser_init(tile_lexer_t* lexer) {
    tile_parser_t parser = {
        .lexer = lexer,
        .prev_token = tile_token_create(TOKEN_NONE, NULL),
        .current_token = tile_token_create(TOKEN_NONE, NULL),
        .next_token = tile_lexer_get_next_token(lexer),
    };
    return parser;
}

void tile_parser_eat(tile_parser_t* parser, token_type_t token_type) {
    if (parser->current_token.type != token_type) {
        printf(
            "Unexpected token %s, with type %d",
            parser->current_token.value,
            parser->current_token.type
        );
    } else {
        parser->prev_token = parser->current_token;
        parser->current_token = parser->next_token;
        parser->next_token = tile_lexer_get_next_token(parser->lexer);
    }
}

tile_ast_t* tile_parser_parse_expression(tile_parser_t* parser);

tile_ast_t* tile_parser_parse_statement(tile_parser_t* parser);

tile_ast_t* tile_parser_parse_statements(tile_parser_t* parser);

tile_ast_t* tile_parser_parse_while_statement(tile_parser_t* parser);

tile_ast_t* tile_parser_parse_if_statement(tile_parser_t* parser);

tile_ast_t* tile_parser_parse_variable_dec_statement(tile_parser_t* parser);

tile_ast_t* tile_parser_parse_block(tile_parser_t* parser);