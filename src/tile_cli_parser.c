#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tile_cli_parser.h>
#include <tile_lexer.h>
#include <tile_parser.h>
#include <tile_ast.h>

#define VERSION "1.0.0"
#define HELP_TEXT \
"Usage: tile [OPTIONS]\n" \
"Options:\n" \
"  --version            Show version information\n" \
"  --help               Show this help message\n" \
"  -f, --file <file>    Specify the input file to compile\n" \
"  -o, --output <file>  Specify the output file\n"

void print_version(void) {
    printf("Tile version %s\n", VERSION);
}

void print_help(void) {
    printf("%s", HELP_TEXT);
}

void handle_input_output(const char* input_file, const char* output_file) {
    FILE* infile = NULL;
    FILE* outfile = NULL;
    char buffer[256];
    tile_lexer_t lexer;
    
    if (input_file) {
        infile = fopen(input_file, "r");
        if (infile == NULL) {
            fprintf(stderr, "Error: Could not open input file %s\n", input_file);
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Error: No input file specified.\n");
        exit(EXIT_FAILURE);
    }

    if (output_file) {
        outfile = fopen(output_file, "w");
        if (outfile == NULL) {
            fprintf(stderr, "Error: Could not open output file %s\n", output_file);
            fclose(infile);
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Error: No output file specified.\n");
        fclose(infile);
        exit(EXIT_FAILURE);
    }

    // Read from the input file, process, and write to the output file
    while (fgets(buffer, sizeof(buffer), infile)) {
        lexer = tile_lexer_init(buffer, NULL);
        
        tile_ast_arena_init();
        tile_parser_t parser = tile_parser_init(&lexer);
        tile_parser_eat(&parser, TOKEN_NONE);
        tile_ast_t* root = tile_parser_parse_statements(&parser);
        tile_ast_show(root, 0);
    
        tile_ast_arena_destroy();
    }

    fclose(infile);
    fclose(outfile);
}

void parse_args(int argc, char *argv[]) {
    const char* input_file = NULL;
    const char* output_file = NULL;

    if (argc == 1) // if there is no input file provided return
        return;
    // if (argc == 1) { // If no arguments are provided, show help by default
    //     print_help();
    //     exit(EXIT_FAILURE);
    // }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            print_version();
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_help();
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[i], "--file") == 0 || strcmp(argv[i], "-f") == 0) {
            // Check if the next argument exists and isn't another option
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                input_file = argv[++i];  // Store input file path
            } else {
                fprintf(stderr, "Error: --file or -f requires a file argument.\n");
                print_help();
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[i], "--output") == 0 || strcmp(argv[i], "-o") == 0) {
            // Check if the next argument exists and isn't another option
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                output_file = argv[++i];  // Store output file path
            } else {
                fprintf(stderr, "Error: --output or -o requires a file argument.\n");
                print_help();
                exit(EXIT_FAILURE);
            }
        } else {
            // Unrecognized option
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_help();
            exit(EXIT_FAILURE);
        }
    }

    handle_input_output(input_file, output_file);

}
