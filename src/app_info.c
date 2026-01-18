// This file defines functions for handling not just the AppInfo struct, but also highlights!

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "wapl.h"
#include "util.c"

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
        .length = WAPL_HL_BEYOND_DEFAULT,
        .capacity = 0,
        .heap_cow = false,
    };

    returned.enable_highlighting = true;
    return returned;
}

// copy-pasted from wapl_newAppCustom with a few changes
wapl_Highlights wapl_newHighlightsCustom (
    const wapl_HighlightKeyValue *const keyvals, size_t length
) {
    assert(keyvals != NULL);
    wapl_Highlights returned = wapl_newHighlights();

    for(size_t i = 0; i < length; i++) {
        const size_t key = keyvals[i].key;
        returned.length = returned.length < key ? key : returned.length;

        if(key < WAPL_HIGHLIGHTS_CAPACITY) {
            returned.array[i] = keyvals[i].value;
            continue;
        }

        const size_t extra_index = key - WAPL_HIGHLIGHTS_CAPACITY;
        returned.extra = reallocUntilFits (
            returned.extra,
            &returned.capacity,
            WAPL_HIGHLIGHTS_CAPACITY,
            extra_index + 1
        );

        returned.extra[extra_index] = keyvals[i].value;
    }

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

    if(key >= highlights->length) {
        return NULL; // TODO: Turn this into a panic. Any out-of-bounds problem here is a mistake.
    }

    if(key >= WAPL_HIGHLIGHTS_CAPACITY) {
        return highlights->array + key;
    }

    return highlights->extra + key - WAPL_HIGHLIGHTS_CAPACITY;
}

wapl_BufferWriteResult wapl_getHighlightString (
    wapl_Highlights *const highlights, size_t key, char *const buffer, size_t buffer_length
) {
    assert(highlights != NULL);
    assert(buffer != NULL);

    const wapl_Highlight *const hl = wapl_getHighlight(highlights, key);
    char *write_point = buffer;
    size_t remaining = buffer_length;

    for(size_t i = 0; i < hl->part_count; i++) {
        // We need an iterator object for getting each of the strings out of a highlight.
        if(hl->parts[i].kind == WAPL_HL_KIND_STRING) {
            const char *string = hl->parts[i].as.string;
            const size_t length = strlen(string); // Getting the length of the string each time we have to
                                                  // use this highlight is annoying and could hurt
                                                  // performance.
            if(length >= remaining) {
                memcpy(write_point, string, remaining - 1);
                write_point[remaining - 1] = '\0';
                return (wapl_BufferWriteResult) {.fits = false, .length = buffer_length - 1};
            }

            memcpy(write_point, string, length);
            write_point += length;
            remaining -= length;

            continue;
        }

        // by this point we can reasonably assume we're working with an index. Later I may want to
        // add a check for an invariant and do something in that case.
        const wapl_BufferWriteResult recursive_result = wapl_getHighlightString (
            highlights,
            hl->parts[i].as.index,
            write_point,
            remaining
        );

        if(!recursive_result.fits) {
            return recursive_result;
        }

        write_point += recursive_result.length;
        remaining -= recursive_result.length;
    }

    *write_point = '\0';
    return (wapl_BufferWriteResult) {
        .fits = true,
        .length = buffer_length - remaining,
    };
}

wapl_BufferWriteResult wapl_highlightString (
    wapl_Highlights *const highlights, size_t key, char *const buffer,
    size_t buffer_length, char *const input
) {
    assert(highlights != NULL);
    assert(buffer != NULL);
    assert(input != NULL);

    wapl_BufferWriteResult result = wapl_getHighlightString (
        highlights,
        WAPL_HL_DEFAULT,
        buffer,
        buffer_length
    );

    if(!result.fits) {
        return (wapl_BufferWriteResult) {.fits = false, .length = buffer_length - 1};
    }
    
    char *write_point = buffer + result.length;
    size_t remaining = buffer_length - result.length;

    result = wapl_getHighlightString(highlights, key, write_point, remaining);
    if(!result.fits) {
        return (wapl_BufferWriteResult) {.fits = false, .length = buffer_length - 1};
    }

    write_point += result.length;
    remaining -= result.length;

    result = wapl_getHighlightString (
        highlights,
        WAPL_HL_DEFAULT,
        write_point,
        remaining
    );

    if(!result.fits) {
        return (wapl_BufferWriteResult) {.fits = false, .length = buffer_length - 1};
    }

    return (wapl_BufferWriteResult) {
        .fits = true,
        .length = buffer_length - remaining + result.length,
    };
}

static wapl_Highlight *prepareAddingHighlight(wapl_Highlights *const highlights, size_t key) {
    // TODO: Add a check for the key being too large, and do a runtime panic is so.

    if(key >= WAPL_HIGHLIGHTS_CAPACITY) {
        const size_t extra_index = key - WAPL_HIGHLIGHTS_CAPACITY;

        if(extra_index >= highlights->capacity) {
            // TODO: realloc() or use some similar function for highlights->extra.
        }

        return highlights->extra + extra_index;
    }

    return highlights->array + key;
}

void wapl_setHighlight(wapl_Highlights *const highlights, size_t key, wapl_HighlightPart value) {
    assert(highlights != NULL);
    *prepareAddingHighlight(highlights, key) = wapl_newHighlight(value);
}

void wapl_setHighlightFull(wapl_Highlights *const highlights, size_t key, wapl_Highlight highlight) {
    assert(highlights != NULL);
    *prepareAddingHighlight(highlights, key) = highlight;
}

void wapl_forceHighlighting(wapl_Highlights *const highlights, bool value) {
    assert(highlights != NULL);
    highlights->enable_highlighting = value;
}

wapl_Context wapl_newApp(wapl_Highlights *const highlights, wapl_AppInfoBuilder app_mask) {
    assert(highlights != NULL);

    #define mMask(field) (app_mask. field ? app_mask. field : "")

    return (wapl_Context) {
        .colors = highlights,
        .app_info = {
            .array = {
                [WAPL_APPINFO_NAME] = mMask(name),
                [WAPL_APPINFO_AUTHOR] = mMask(author),
                [WAPL_APPINFO_VERSION] = mMask(version),
                [WAPL_APPINFO_URL] = mMask(url),
                [WAPL_APPINFO_SHORT_DESCRIPTION] = mMask(short_description),
                [WAPL_APPINFO_LONG_DESCRIPTION] = mMask(long_description),
            },
            .extra = NULL,
            .capacity = 0,
            .length = WAPL_APPINFO_BEYOND_DEFAULT,
        },
    };

    #undef mMask
}

wapl_Context wapl_newAppCustom (
    wapl_Highlights *const highlights,
    wapl_AppInfoBuilder app_mask,
    const wapl_AppInfoKeyValue *const keyvals,
    size_t keyvals_length
) {
    assert(highlights != NULL);

    wapl_Context returned = wapl_newApp(highlights, app_mask);

    for(size_t i = 0; i < keyvals_length; i++) {
        const size_t key = keyvals[i].key;
        returned.app_info.length = returned.app_info.length < key ? key : returned.app_info.length;

        if(key < WAPL_APP_INFO_CAPACITY) {
            returned.app_info.array[i] = keyvals[i].value;
            continue;
        }

        const size_t extra_index = key - WAPL_APP_INFO_CAPACITY;
        returned.app_info.extra = reallocUntilFits (
            returned.app_info.extra,
            &returned.app_info.capacity,
            WAPL_APP_INFO_CAPACITY,
            extra_index + 1
        );

        returned.app_info.extra[extra_index] = keyvals[i].value;
    }

    return returned;
}

void deleteHighlights(wapl_Highlights *const highlights) {
    assert(highlights != NULL);

    if(highlights->extra) {
        free(highlights->extra);
    }

    *highlights = (wapl_Highlights) {0};
}

void deleteApp(wapl_Context *const context) {
    assert(context != NULL);
    deleteHighlights(context->colors);

    if(context->app_info.extra) {
        free(context->app_info.extra);
    }

    context->app_info = (wapl_AppInfo) {0};
}

