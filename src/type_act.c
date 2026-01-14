#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "wapl.h"

// Hard-coding a buffer size is problematic for any string we pass in that's too big. A custon
// vector-like data structure would be handly, specifically one that puts stuff on the stack if the
// data is small enough.
#define BIG_ENOUGH_BUFFER 1024

// What should a NULL converter mean? Should it just cause a crash? Or should it be handled like a
// string?
mConverter(convert_int) {
    size_t copy_size = BIG_ENOUGH_BUFFER > param_length ? param_length : BIG_ENOUGH_BUFFER;
    char buffer[BIG_ENOUGH_BUFFER];
    memcpy(buffer, param, copy_size); // This is buggy. I need something a little smarter than
                                      // just a raw statically allocated buffer. Or better yet,
                                      // functions for integer conversion from a length-prefixed
                                      // string...
    buffer[copy_size - 1] = '\0';

    int target_tmp = 0;
    if(sscanf(buffer, "%d", &target_tmp) == EOF) { // I should use strtol instead. Honestly. it's
        // Imagining some made up API for a compound error...
        wapl_addError(errors, strerror(errno)); // brooooo how tf will this API work? I need extra
                                                // info to provide the error the ability tgo print
                                                // info pointing to a problematic token in the arg
                                                // list. Is it dynamic dispatch time? That seems
                                                // like a good way to solve this.

        return; // I may want to return a boolean to indicate function failure. Or provide at least
                // some easy way to get if a new error was added by this function.
    }

    *(int*)target = target_tmp;
    return;
}

const wapl_Type wapl_type_c_int = {
    .name = "C int",
    .highlighting = "",
    .converter = &convert_int,
};

const wapl_Type wapl_type_c_long = {
    .name = "C long",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_c_short = {
    .name = "C short",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_c_char = {
    .name = "char",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_i8 = {
    .name = "int8",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_i16 = {
    .name = "int16",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_i32 = {
    .name = "int32",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_i64 = {
    .name = "int64",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_isize = {
    .name = "intmax",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_u8 = {
    .name = "u8",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_u16 = {
    .name = "u16",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_u32 = {
    .name = "u32",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_u64 = {
    .name = "u64",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_usize = {
    .name = "uint",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_f32 = {
    .name = "float",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_f64 = {
    .name = "double",
    .highlighting = "",
    .converter = NULL,
};

const wapl_Type wapl_type_string = {
    .name = "string",
    .highlighting = "",
    .converter = NULL,
};

