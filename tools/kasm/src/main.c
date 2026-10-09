#include <stdio.h>
#include "common/source.h"
#include <stdlib.h>
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return -1;
    }

    Source source;
    if (source_load(&source, argv[1]) != 0) {
        printf("Failed to load source file: %s\n", argv[1]);
        return -1;
    }

    printf("Loaded source file: %s\n", source.path);
    printf("Source length: %zu\n", source.length);
    printf("Source content:\n%s\n", source.text);

    free(source.text);

    return 0;
}