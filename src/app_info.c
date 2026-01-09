// This file defines functions for handling not just the AppInfo struct, but also color pallettes!

#include "wapl.h"

// Can't wait to come back to this codebase later to see my macro abuse and die inside
#define mMask(field, default_value) \
    .field = MASK_NAME.field == 0 ? default_value : MASK_NAME.field

wapl_ColorPallette wapl_makePallette(wapl_ColorPallette mask) {
#define MASK_NAME mask
    return (wapl_ColorPallette) {
        mMask(color_info, "\33[1m"),
        mMask(color_warn, "\33[1;93m"),
        mMask(color_error, "\33[1;95m"),
        mMask(color_fatal, "\33[1;91m"),
    };
#undef MASK_NAME
}

wapl_Context wapl_makeApp(wapl_ColorPallette *const colors, wapl_AppInfo app_mask) {
#define MASK_NAME app_mask
    return (wapl_Context) {
        .colors = colors,
        .app_info = {
            mMask(name, "my_wapl_app"),
            mMask(author, "John Doe"),
            mMask(version, "0.0.1"),
            mMask(url, "https://www.youtube.com/watch?v=dQw4w9WgXcQ"),
            mMask(short_description, "Default short description. If you see this, this app has a bug!"),
            mMask(long_description, "Default long description. If you see this, this app has a bug!"),
        },
    };
#undef MASK_NAME
}

#undef mApplyMask
