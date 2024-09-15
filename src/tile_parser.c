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
            .number.value = atoi(parser->current_token.value), // atoi: string -> int casting
            .tag = AST_LITERAL_INT,
        });
        break;
    
    case TOKEN_FLOAT_LITERAL:
        tile_parser_eat(parser, TOKEN_FLOAT_LITERAL);
        return tile_ast_create((tile_ast_t) {
            .number.text_value = parser->current_token.value,
            .number.fvalue = atof(parser->current_token.value),
            .tag = AST_LITERAL_FLOAT,
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

    case TOKEN_IF:
        return tile_parser_parse_if_statement(parser);
        break;

    case TOKEN_MATCH:
        return tile_parser_parse_match_statement(parser);
        break;

    case TOKEN_INT_KW:
        return tile_parser_parse_variable_dec_statement(parser);
        break;

    case TOKEN_FLOAT_KW:
        return tile_parser_parse_variable_dec_statement(parser);
        break; 

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
    // while (expression) 
    // { } // block part 

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

tile_ast_t* tile_parser_parse_if_statement(tile_parser_t* parser) {
    // if (condition) 
    // { } // block part 

    tile_parser_eat(parser, TOKEN_IF);
    tile_parser_eat(parser, TOKEN_LPAREN);
    tile_ast_t* condition = tile_parser_parse_expression(parser); // condition part
    tile_parser_eat(parser, TOKEN_RPAREN);
    tile_ast_t* body = tile_parser_parse_block(parser);
    
    tile_ast_t* altarnate = NULL;
    if (parser->current_token.type == TOKEN_ELSE) { // 'else'
        tile_parser_eat(parser, TOKEN_ELSE);
        if (parser->current_token.type == TOKEN_IF) {  // 'else if'
            altarnate = tile_parser_parse_if_statement(parser);
        }
        else { // There is no 'if', only 'else'
            altarnate = tile_parser_parse_block(parser);
        }
    }

    tile_ast_t* if_statement = tile_ast_create((tile_ast_t){
        .if_statement.condition = condition,
        .if_statement.body = body,
        .if_statement.altarnate = altarnate,
        .tag = AST_IF_STATEMENT,
    });
    
    return if_statement;
}

tile_ast_t* tile_parser_parse_match_statement(tile_parser_t* parser) {
    // match (expression) {
    //     option condition:
    //         statements
    //     option condition:
    //         statements
    //     default:
    //         statements
    // }
    tile_parser_eat(parser, TOKEN_MATCH);
    tile_parser_eat(parser, TOKEN_LPAREN);
    tile_ast_t* expression = tile_parser_parse_expression(parser);
    tile_parser_eat(parser, TOKEN_RPAREN);
    size_t default_number = 0;

    tile_parser_eat(parser, TOKEN_LBRACE);
    tile_ast_t** options = NULL;
    tile_ast_t* default_option;
    while (parser->current_token.type != TOKEN_RBRACE) {
        if (parser->current_token.type == TOKEN_OPTION) {
            tile_ast_t* option = tile_parser_parse_option(parser);
            arrput(options, option);
        }
        else if (parser->current_token.type == TOKEN_DEFAULT) {
            default_option = tile_parser_parse_default_option(parser);
            default_number++;
        }
    }
    tile_parser_eat(parser, TOKEN_RBRACE);
    
    tile_ast_t* match_statement = tile_ast_create((tile_ast_t) {
        .match_statement.expression = expression,
        .match_statement.options = options,
        .match_statement.default_option = default_option,
        .match_statement.option_count = arrlen(options) + default_number,
        .tag = AST_MATCH_STATEMENT,
    });
    printf("Total count %lld \n", match_statement->match_statement.option_count);
    return match_statement;
}

tile_ast_t* tile_parser_parse_option(tile_parser_t* parser) {
    // option condition:
    //     statements
    // option condition:
    //     statements
    tile_parser_eat(parser, TOKEN_OPTION);
    tile_ast_t* condition = tile_parser_parse_expression(parser);
    tile_parser_eat(parser, TOKEN_COLON);
    tile_ast_t** statements = NULL;

    while (parser->current_token.type != TOKEN_OPTION 
          && parser->current_token.type != TOKEN_DEFAULT 
          && parser->current_token.type != TOKEN_RBRACE) {
        tile_ast_t* statement;
        statement = tile_parser_parse_statement(parser);
        arrput(statements, statement);
    }

    tile_ast_t* option = tile_ast_create((tile_ast_t) {
        .option_statement.condition = condition,
        .option_statement.statements = statements,
        .option_statement.statement_count = arrlen(statements),
        .tag = AST_OPTION_STATEMENT,
    });

    return option;
}

tile_ast_t* tile_parser_parse_default_option(tile_parser_t* parser) {
    // default:
    //     statements
    tile_parser_eat(parser, TOKEN_DEFAULT);
    tile_parser_eat(parser, TOKEN_COLON);
    tile_ast_t** statements = NULL;
    while (parser->current_token.type != TOKEN_RBRACE) {
        tile_ast_t* statement;
        statement = tile_parser_parse_statement(parser);
        arrput(statements, statement);
    }
    tile_ast_t* default_option = tile_ast_create((tile_ast_t) {
        .default_statement.statements = statements,
        .default_statement.statement_count = arrlen(statements),
        .tag = AST_DEFAULT_STATEMENT,
    });
    return default_option;
}

tile_ast_t* tile_parser_parse_variable_dec_statement(tile_parser_t* parser) {
    // int x = 10;
    // float x;

    const char* type_name = parser->current_token.value;
    if (parser->current_token.type == TOKEN_INT_KW)
        tile_parser_eat(parser, TOKEN_INT_KW);
    else if (parser->current_token.type == TOKEN_FLOAT_KW)
        tile_parser_eat(parser, TOKEN_FLOAT_KW);

    const char* var_name = parser->current_token.value;
    tile_parser_eat(parser, TOKEN_ID);

    tile_ast_t* init_expr = NULL;
    if (parser->current_token.type == TOKEN_ASSIGN) {
        tile_parser_eat(parser, TOKEN_ASSIGN);

        init_expr = tile_parser_parse_expression(parser);
    }

    tile_parser_eat(parser, TOKEN_SEMI);

    tile_ast_t* var_decl_statement = tile_ast_create((tile_ast_t) {
        .variable_decl.type = type_name,
        .variable_decl.name = var_name,
        .tag = AST_VARIABLE_DECL,
    });

    if (init_expr != NULL) {
        tile_ast_t* var_assign_statement = tile_ast_create((tile_ast_t) {
            .variable_assign.type = type_name,
            .variable_assign.name = var_name,
            .variable_assign.value = init_expr,
            .tag = AST_VARIABLE_ASSIGN,
        });
        return var_assign_statement;
    }

    return var_decl_statement;
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