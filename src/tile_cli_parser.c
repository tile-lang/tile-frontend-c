#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>  // Use getopt_long instead of getopt for handle long options
#include <tile_cli_parser.h>

#define VERSION "1.0.0"
#define HELP_TEXT \
"Usage: tile.exe [OPTIONS]\n" \
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
    // Structure defining the long options
    static struct option long_options[] = {
        {"version", no_argument, NULL, 'v'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "vh"/*short options*/, long_options, NULL)) != -1) {
        switch (opt) {
            case 'v':
                print_version();
                exit(EXIT_SUCCESS);
            case 'h':
                print_help();
                exit(EXIT_SUCCESS);
            default:
                print_help();
                exit(EXIT_FAILURE);
        }
    }

    // Check for remaining arguments that are not options
    if (optind < argc) {
        fprintf(stderr, "Unknown option: %s\n", argv[optind]);
        print_help();
        exit(EXIT_FAILURE);
    }
}
