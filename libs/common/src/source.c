#include "common/source.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int source_load(Source *source, const char *path) {
    if (!source || !path) {
        return -1;
    }
    FILE *file = fopen(path, "rb");
    if (!file) {
        return -1;
    }
    fseek(file, 0, SEEK_END);
    source->length = ftell(file);
    fseek(file, 0, SEEK_SET);
    source->text = (char *)malloc(source->length + 1);
    if (!source->text) {
        fclose(file);
        return -1;
    }
    fread(source->text, 1, source->length, file);
    source->text[source->length] = '\0';
    fclose(file);
    source->path = path;
    return 0;
}