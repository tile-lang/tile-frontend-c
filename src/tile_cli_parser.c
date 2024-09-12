#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tile_cli_parser.h>

#define VERSION "1.0.0"
#define HELP_TEXT \
"Usage: tile [OPTIONS]\n" \
"Options:\n" \
"  --version      Show version information\n" \
"  --help         Show this help message\n"

void print_version(void) {
    printf("Tile version %s\n", VERSION);
}

void print_help(void) {
    printf("%s", HELP_TEXT);
}

void parse_args(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            print_version();
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_help();
            exit(EXIT_SUCCESS);
        } else {
            // Unrecognized option
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_help();
            exit(EXIT_FAILURE);
        }
    }
}
