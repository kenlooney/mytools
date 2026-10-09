#include "common/cursor.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    Source source;
    if (argc != 2) {
        fprintf(stderr, "usage: lesson input.asm\n");
        return 1;
    }
    if (source_load(&source, argv[1]) != 0) {
        fprintf(stderr, "Failed to load source file: %s\n", argv[1]);
        return 1;
    }

    Cursor cursor = cursor_start(&source);
    while (cursor_peek(&cursor) != '\0') {
        printf("%zu:%zu byte %u\n", cursor.line, cursor.column,
               (unsigned int)(unsigned char)cursor_peek(&cursor));
        cursor_advance(&cursor);
    }
    free(source.text);
    return 0;
}
