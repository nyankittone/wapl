#include "wapl.h"

// Merging all of these bits of data to force data locality could be a good idea.

// Default app metadata parameters. Change these values to something appropriate for your app :3
const char *wapl_name = "default_wapl_app";
const char *wapl_author = "John Doe";
const char *wapl_version = "0.0.1";
const char *wapl_url = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";

wapl_Context wapl_default_context = {
    .color_info = "\33[1m",
    .color_warn = "\33[1;93m",
    .color_error = "\33[1;91m",
    .color_fatal = "\33[1;91m",
};

