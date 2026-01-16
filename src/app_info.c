// This file defines functions for handling not just the AppInfo struct, but also highlights!

#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "wapl.h"

wapl_HighlightPart wapl_hlString(char *const string) {
    return (wapl_HighlightPart) {
        .kind = WAPL_HL_KIND_STRING,
        .as.string = string,
    };
}

wapl_HighlightPart wapl_hlIndex(size_t index) {
    return (wapl_HighlightPart) {
        .kind = WAPL_HL_KIND_INDEX,
        .as.index = index,
    };
}

wapl_Highlight wapl_newHighlight(wapl_HighlightPart part) {
    return (wapl_Highlight) {
        .part_count = 1,
        .parts = {part},
    };
}

wapl_Highlight wapl_newHighlightFull(wapl_HighlightPart *const parts, size_t length) {
    assert(parts != NULL);
    assert(length <= WAPL_HIGHLIGHT_MAX_PARTS);

    wapl_Highlight returned = {0};
    returned.part_count = length;
    memcpy(&returned.part_count, parts, sizeof(wapl_HighlightPart) * length);

    return returned;
}

wapl_Highlight wapl_newHighlightVar(size_t count, ...) {
    assert(count <= WAPL_HIGHLIGHT_MAX_PARTS);

    wapl_Highlight returned = {
        .part_count = count,
    };

    va_list parts;
    va_start(parts, count);

    // Copying each highlight part in a loop feels smelly. But to the best of my knowledge, this is
    // the only good way of doing this in a portable manner.
    for(size_t i = 0; i < count; i++) {
        returned.parts[i] = va_arg(parts, wapl_HighlightPart);
    }

    va_end(parts);
    return returned;
}

wapl_Highlights wapl_newHighlights(void) {
    wapl_Highlights returned = {
        .array = {
            [WAPL_HL_DEFAULT] = wapl_newHighlight(wapl_hlString("")),
            [WAPL_HL_BOLD] = wapl_newHighlight(wapl_hlString("\33[1m")),
            [WAPL_HL_ITALIC] = wapl_newHighlight(wapl_hlString("\33[3m")),
            [WAPL_HL_UNDERLINE] = wapl_newHighlight(wapl_hlString("\33[4m")),
            [WAPL_HL_BLINK] = wapl_newHighlight(wapl_hlString("\33[5m")),
            [WAPL_HL_BLACK] = wapl_newHighlight(wapl_hlString("\33[30m")),
            [WAPL_HL_RED] = wapl_newHighlight(wapl_hlString("\33[31m")),
            [WAPL_HL_GREEN] = wapl_newHighlight(wapl_hlString("\33[32m")),
            [WAPL_HL_YELLOW] = wapl_newHighlight(wapl_hlString("\33[33m")),
            [WAPL_HL_BLUE] = wapl_newHighlight(wapl_hlString("\33[34m")),
            [WAPL_HL_MAGENTA] = wapl_newHighlight(wapl_hlString("\33[35m")),
            [WAPL_HL_CYAN] = wapl_newHighlight(wapl_hlString("\33[36m")),
            [WAPL_HL_LIGHT_GRAY] = wapl_newHighlight(wapl_hlString("\33[37m")),
            [WAPL_HL_DARK_GRAY] = wapl_newHighlight(wapl_hlString("\33[90m")),
            [WAPL_HL_LIGHT_RED] = wapl_newHighlight(wapl_hlString("\33[91m")),
            [WAPL_HL_LIGHT_GREEN] = wapl_newHighlight(wapl_hlString("\33[92m")),
            [WAPL_HL_LIGHT_YELLOW] = wapl_newHighlight(wapl_hlString("\33[93m")),
            [WAPL_HL_LIGHT_BLUE] = wapl_newHighlight(wapl_hlString("\33[94m")),
            [WAPL_HL_LIGHT_MAGENTA] = wapl_newHighlight(wapl_hlString("\33[95m")),
            [WAPL_HL_LIGHT_CYAN] = wapl_newHighlight(wapl_hlString("\33[96m")),
            [WAPL_HL_WHITE] = wapl_newHighlight(wapl_hlString("\33[97m")),
            [WAPL_HL_INFO] = wapl_newHighlight(wapl_hlIndex(WAPL_HL_BOLD)),
            [WAPL_HL_WARN] = wapl_newHighlightVar(2, wapl_hlIndex(WAPL_HL_BOLD),wapl_hlIndex(WAPL_HL_LIGHT_YELLOW)),
            [WAPL_HL_ERROR] = wapl_newHighlightVar(2, wapl_hlIndex(WAPL_HL_BOLD), wapl_hlIndex(WAPL_HL_LIGHT_RED)),
            [WAPL_HL_FATAL] = wapl_newHighlightVar(2, wapl_hlIndex(WAPL_HL_BOLD), wapl_hlIndex(WAPL_HL_LIGHT_RED)),
            [WAPL_HL_NAME] = wapl_newHighlightVar(2, wapl_hlIndex(WAPL_HL_BOLD), wapl_hlIndex(WAPL_HL_LIGHT_CYAN)),
            [WAPL_HL_AUTHOR] = wapl_newHighlight(wapl_hlIndex(WAPL_HL_ITALIC)),
            [WAPL_HL_VERSION] = wapl_newHighlightVar(2, wapl_hlIndex(WAPL_HL_BOLD), wapl_hlIndex(WAPL_HL_LIGHT_BLUE)),
            [WAPL_HL_URL] = wapl_newHighlight(wapl_hlIndex(WAPL_HL_UNDERLINE)),
        },
        .extra = NULL,
        .largest_index = WAPL_HL_BEYOND_DEFAULT,
        .capacity = 0,
        .heap_cow = false,
    };

    returned.enable_highlighting = true;
    return returned;
}

wapl_Highlights wapl_copyHighlights(const wapl_Highlights *const from) {
    assert(from != NULL);

    wapl_Highlights returned = *from;
    returned.heap_cow = true;
    return returned;
}

const wapl_Highlight *wapl_getHighlight(wapl_Highlights *const highlights, size_t key) {
    assert(highlights != NULL);

    if(key >= highlights->largest_index) {
        return NULL; // TODO: Turn this into a panic or let us wrap the return in an Option<T> thing
    }

    if(key >= WAPL_HIGHLIGHTS_CAPACITY) {
        return highlights->array + key;
    }

    return highlights->extra + key - WAPL_HIGHLIGHTS_CAPACITY;
}

wapl_BufferWriteResult wapl_getHighlightString (
    wapl_Highlights *const highlights, size_t key, char *const buffer, size_t buffer_length
) {
}

void wapl_forceHighlighting(wapl_Highlights *const highlights, bool value) {
    assert(highlights != NULL);
    highlights->enable_highlighting = value;
}

