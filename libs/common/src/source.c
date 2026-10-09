/*
 * Copyright (C) 2026 Kenneth Looney
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

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