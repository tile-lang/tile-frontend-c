#define ARENA_IMPLEMENTATION
#include <tile_lexer.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <common/arena.h>
#include <common/cmd_colors.h>
#include <stb_ds.h>

#define LEXER_ARENA_CAPACITY 2048

typedef struct tile_lextoken_t {
    char* text;
    token_type_t type;
} tile_lextoken_t;

#define ARR_LEN(x) sizeof(x)/sizeof(x[0])

static const tile_lextoken_t symbols[] = {
    { "<", TOKEN_LESS },
    { "<=", TOKEN_LESSEQ },
    { ">", TOKEN_GREATER },
    { ">=", TOKEN_GREATEREQ },
    { "==", TOKEN_EQ },
    { "!=", TOKEN_NEQ },

    { "(", TOKEN_LPAREN },
    { ")", TOKEN_RPAREN },
    { "{", TOKEN_LBRACE },
    { "}", TOKEN_RBRACE },
    { "[", TOKEN_LSQUARE },
    { "]", TOKEN_RSQUARE },

    { "=", TOKEN_ASSIGN },
    { ":", TOKEN_COLON },
    { ";", TOKEN_SEMI },
    { ",", TOKEN_COMMA },
    { "//", TOKEN_COMMENT },

    { "+", TOKEN_PLUS },
    { "-", TOKEN_MINUS },
    { "*", TOKEN_STAR },
    { "/", TOKEN_SLASH },

    { "++", TOKEN_INC },
    { "--", TOKEN_DEC },

    { "unknown", TOKEN_UNKNOWN },
    { NULL, TOKEN_NONE },
};

static const tile_lextoken_t keywords[] = {
    { "true", TOKEN_TRUE_LITERAL },
    { "false", TOKEN_FALSE_LITERAL },

    { "if", TOKEN_IF },
    { "else", TOKEN_ELSE },

    { "for", TOKEN_FOR },
    { "while", TOKEN_WHILE },

    { "match", TOKEN_MATCH },
    { "option", TOKEN_OPTION },
    { "default", TOKEN_DEFAULT },
    { "break", TOKEN_BREAK },

    { "func", TOKEN_TYPE_KW },
    { "return", TOKEN_RETURN },

    { "int", TOKEN_TYPE_KW },
    { "float", TOKEN_TYPE_KW },

    { NULL, TOKEN_NONE },
};

tile_lexer_t tile_lexer_init(const char* src, const char* file_name) {
    tile_lexer_t lexer = {
        .cursor = 0,
        .prev_char = '\0',
        .current_char = '\0',
        .next_char = src[0],
        .source_code = src,
        .source_code_size = strlen(src),
        .line_lengths = NULL,
        .loc.row = 0,
        .loc.col = 0,
        .loc.file_name =  file_name,
        .arena = arena_init(LEXER_ARENA_CAPACITY),
    };
    return lexer;
}

void tile_lexer_destroy(tile_lexer_t* lexer) {
    arena_destroy(lexer->arena);
    arrfree(lexer->line_lengths);
}

void tile_lexer_advance(tile_lexer_t* lexer) {
    if (lexer->current_char == '\n') {
        lexer->loc.row++;
        lexer->loc.col = 0;
    }
    if (!tile_lexer_is_at_end(lexer)) {
        lexer->cursor++;
        lexer->prev_char = lexer->current_char;
        lexer->current_char = lexer->next_char;
        lexer->next_char = lexer->source_code[lexer->cursor];
        lexer->loc.col++;
    }
}

void tile_lexer_advance_by(tile_lexer_t* lexer, size_t steps) {
    //assert() if no way to advance that step
    for (size_t i = 0; i < steps; i++) {
        tile_lexer_advance(lexer);
    }
}

void tile_lexer_skip_whitespace(tile_lexer_t* lexer) {
    while (isspace(lexer->current_char)) {
        if (lexer->current_char == '\n')
            arrput(lexer->line_lengths, lexer->loc.col);
        tile_lexer_advance(lexer);
    }
}

void tile_lexer_skip_line(tile_lexer_t* lexer) {
    while (lexer->current_char != '\n' && lexer->current_char != EOF)
        tile_lexer_advance(lexer);
}

// Peek function checks the next character without advancing
static char tile_lexer_peek_upgraded(tile_lexer_t* lexer, bool reset) {
    static size_t peek = 0;
    if(peek + lexer->cursor >= lexer->source_code_size)
        return '\0';
    peek++;
    if (reset)
        peek = 0;
    return lexer->source_code[lexer->cursor + peek - 1];
}

char tile_lexer_peek(tile_lexer_t *lexer) {
    return tile_lexer_peek_upgraded(lexer, false);
}

void tile_lexer_peek_reset(tile_lexer_t *lexer) {
    tile_lexer_peek_upgraded(lexer, true);
}

size_t tile_lexer_peek_until_endline(tile_lexer_t* lexer, char* character) {
    size_t count = 0;
    *character = 0;
    while (*character != '\n' && *character != EOF) {
        *character = tile_lexer_peek(lexer);
        count++;
    }
    tile_lexer_peek_reset(lexer);
    return count;
}

const char* tile_lexer_get_line(tile_lexer_t *lexer, size_t row, size_t* character_count) {
    // lexer->source_code[lexer->cursor - 1];
    // lexer->line_lengths[lexer->loc.row - 1];
    (void)lexer;
    (void)row;
    (void)character_count;
    return NULL;
}

tile_token_t tile_lexer_get_next_token(tile_lexer_t* lexer) {
    if (isspace(lexer->current_char))
        tile_lexer_skip_whitespace(lexer);
    if (isdigit(lexer->current_char))
        return tile_lexer_collect_number(lexer);
    if (isalpha(lexer->current_char) || lexer->current_char == '_')
        return tile_lexer_collect_id(lexer);
    if (lexer->current_char == '\"')
        return tile_lexer_collect_string(lexer);
    if (lexer->current_char == '/' && tile_lexer_peek(lexer) == '/') {
        tile_lexer_peek_reset(lexer);
        tile_lexer_skip_line(lexer);
        return tile_token_create(TOKEN_COMMENT, "//");
    }

    return tile_lexer_collect_symbol(lexer);
}

tile_token_t tile_lexer_collect_symbol(tile_lexer_t* lexer) {
    for (size_t i = 0; i < ARR_LEN(symbols); i++) {
        if (lexer->current_char == EOF)
            return tile_token_create(TOKEN_EOF, "eof");
        if (symbols[i].type == TOKEN_UNKNOWN){
            tile_lexer_advance(lexer);
            return tile_token_create(TOKEN_UNKNOWN, "unknown");
        }
        if (strlen(symbols[i].text) == 1) {
            if (lexer->current_char == symbols[i].text[0]) {
                tile_lexer_advance(lexer);
                return tile_token_create(symbols[i].type, arena_strdup(lexer->arena, symbols[i].text));
            }
        }
        else if (strlen(symbols[i].text) == 2) {
            if (lexer->current_char == symbols[i].text[0] && lexer->next_char == symbols[i].text[1]) {
                tile_lexer_advance_by(lexer, 2);
                return tile_token_create(symbols[i].type, arena_strdup(lexer->arena, symbols[i].text));
            }
        }
    }
    return tile_token_create(TOKEN_NONE, NULL);
}

tile_token_t tile_lexer_collect_string(tile_lexer_t* lexer) {
    tile_lexer_advance(lexer); // skip opening '"'
    size_t len = 0;
    char temp_val[128];

    size_t line_end = 0;
    char c = tile_lexer_peek(lexer);
    while (c != '\n' && c != EOF) {
        line_end++;
        c = tile_lexer_peek(lexer);
    }
    tile_lexer_peek_reset(lexer);
    while (lexer->current_char != '"') {
         if (len > line_end) {
            printf("%s:%d:%d: "CLR_RED"ERROR"CLR_END" missing string quota '\"'\n",
                lexer->loc.file_name,
                lexer->loc.row,
                lexer->loc.col
            );
            break;
        }
        temp_val[len] = lexer->current_char;
        len++;
        tile_lexer_advance(lexer);
    }
    temp_val[len] = '\0';
    len++;
    char* val = (char*)arena_alloc(&lexer->arena, len);
    memmove(val, temp_val, len);
    tile_lexer_advance(lexer); // skip closing '"'

    return tile_token_create(TOKEN_STRING_LITERAL, val);
}

tile_token_t tile_lexer_collect_id(tile_lexer_t *lexer) {
    size_t len = 0;
    char temp_val[128];

    while (isalnum(lexer->current_char) || lexer->current_char == '_') {
        temp_val[len] = lexer->current_char;
        len++;
        tile_lexer_advance(lexer);
    }
    temp_val[len] = '\0';
    len++;
    char* val = (char*)arena_alloc(&lexer->arena, len);
    memmove(val, temp_val, len);

    for (int i = 0; keywords[i].text != NULL; i++) {
        if (strcmp(val, keywords[i].text) == 0) {
            return tile_token_create(keywords[i].type, keywords[i].text);
        }
    }
    return tile_token_create(TOKEN_ID, val);
}

tile_token_t tile_lexer_collect_number(tile_lexer_t* lexer) {
    size_t len = 0;
    char temp_val[128];
    token_type_t type = TOKEN_FLOAT_LITERAL;

    while(isdigit(lexer->current_char)) {
        temp_val[len] = lexer->current_char;
        len++;
        tile_lexer_advance(lexer);
    }

    if (lexer->current_char == '.') {
        temp_val[len] = lexer->current_char;
        len++;
        tile_lexer_advance(lexer);

        while (isdigit(lexer->current_char)) {
            temp_val[len] = lexer->current_char;
            len++;
            tile_lexer_advance(lexer);
        }   
    }
    else {
        type = TOKEN_INT_LITERAL;
    }
    temp_val[len] = '\0';
    len++;
    char* val = (char*)arena_alloc(&lexer->arena, len);
    memmove(val, temp_val, len);
    tile_token_t token = tile_token_create(type, val);
    return token;
}

int tile_lexer_is_at_end(tile_lexer_t* lexer) {
    return lexer->cursor >= lexer->source_code_size;
}