#include <tile_token.h>
#include <stdlib.h>
#include <string.h>

tile_token_t tile_token_create(token_type_t type, char* value) {
    return (tile_token_t) {
        .type = type,
        .value = value
    };
}