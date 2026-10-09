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
#include "common/cursor.h"

// Initialize a cursor at the start of the given source.
Cursor cursor_start(const Source *source) {
    Cursor cursor;
    cursor.source = source;
    cursor.offset = 0;
    cursor.line = 1;
    cursor.column = 1;
    return cursor;
}
// Peek at the character at the current cursor position. Returns '\0' if at the end of the source.
char cursor_peek(const Cursor *cursor) {
    if (cursor->offset >= cursor->source->length) {
        return '\0';
    }
    return cursor->source->text[cursor->offset];
}

// Advance the cursor by one character, updating line and column information.
void cursor_advance(Cursor *cursor) {
    if (cursor->offset >= cursor->source->length) {
        return;
    }
    if (cursor->source->text[cursor->offset] == '\n') {
        cursor->line++;
        cursor->column = 1;
    } else {
        cursor->column++;
    }
    cursor->offset++;
}