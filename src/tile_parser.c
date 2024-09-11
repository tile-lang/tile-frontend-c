#include <tile_parser.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

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
        // TODO: print row and col of unexpected token
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

tile_ast_t* tile_parser_parse_expression(tile_parser_t* parser) {
    // "test"
    // 3
    switch (parser->current_token.type) {
    
    case TOKEN_INT_LITERAL:
        tile_parser_eat(parser, TOKEN_INT_LITERAL);
        return tile_ast_create((tile_ast_t) {
            .number.text_value = parser->current_token.value,
            .number.value = atoi(parser->current_token.value),
            .tag = AST_LITERAL_INT,
        });
        break;

    default:
    break;
    }
}

tile_ast_t* tile_parser_parse_statement(tile_parser_t* parser) {
    switch (parser->current_token.type) {
    
    case TOKEN_WHILE:
        return tile_parser_parse_while_statement(parser);
        break;

    // case TOKEN_IF:
    //     return tile_parser_parse_if_statement(parser);
    //     break;
        
    // case TOKEN_MATCH:
    //     return tile_parser_parse_match_statement(parser);
    //     break;
    
    default:
    break;
    }
}

tile_ast_t* tile_parser_parse_statements(tile_parser_t* parser) {
    tile_ast_t** statements = NULL;
    while (parser->current_token.type != TOKEN_EOF) {
        tile_ast_t* statement;
        statement = tile_parser_parse_statement(parser);
        arrput(statements, statement);
    }
    tile_ast_t* program = tile_ast_create((tile_ast_t) {
        .program.statements = statements,
        .program.statement_count = arrlen(statements),
        .tag = AST_PROGRAM,
    });

    return program;
}

tile_ast_t* tile_parser_parse_while_statement(tile_parser_t* parser) {
    // while (condition) {
    // body
    // }

    tile_parser_eat(parser, TOKEN_WHILE);
    tile_parser_eat(parser, TOKEN_LPAREN);
    tile_ast_t* condition = tile_parser_parse_expression(parser); // condition part
    tile_parser_eat(parser, TOKEN_RPAREN);
    tile_ast_t* body = tile_parser_parse_block(parser); // body part
    tile_ast_t* while_statement = tile_ast_create((tile_ast_t) {
        .while_statement.condition = condition,
        .while_statement.body = body,
        .tag = AST_WHILE_STATEMENT,
    });
    return while_statement;
}

tile_ast_t* tile_parser_parse_block(tile_parser_t* parser) {
    // {
    //     statements
    // }

    tile_parser_eat(parser, TOKEN_LBRACE);
    tile_ast_t** statements = NULL;
    
    while (parser->current_token.type != TOKEN_RBRACE) {
        tile_ast_t* statement;
        statement = tile_parser_parse_statement(parser);
        arrput(statements, statement);
    }

    tile_parser_eat(parser, TOKEN_RBRACE);

    tile_ast_t* block = tile_ast_create((tile_ast_t) {
        .block.statements = statements,
        .block.statement_count = arrlen(statements),
        .tag = AST_BLOCK,
    });
    return block;
}