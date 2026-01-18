
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// We're using fputs() here instead of fprintf for printing formatted text to reduce the odds of an
// out-of-memory error preventing us from displaying the error.
#define mInternalFatal(message) {\
    fputs("WAPL FATAL ERROR @ ", stderr); \
    fputs(__func__, stderr); \
    fputs(": ", stderr); \
    fputs(message, stderr); \
    fputc('\n', stderr); \
    abort(); \
}

// We need a functions for reallocating memory.
static void *reallocOrDie(void *prev, size_t size) {
    void *new_allocation = realloc(prev, size);
    if(!new_allocation) {
        mInternalFatal("Out of memory!");
    }

    return new_allocation;
}

static void *mallocOrDie(size_t size) {
    return reallocOrDie(NULL, size);
}

static void *reallocUntilFits(void *prev, size_t *capacity, size_t init_capacity, size_t needed_capacity) {
    assert(capacity != NULL);
    if(needed_capacity > *capacity) {
        return prev;
    }

    if(*capacity < 2) {
        *capacity = init_capacity;
    }

    while(*capacity < needed_capacity) {
        *capacity *= 1.5;
    }

    return reallocOrDie(prev, *capacity);
}

