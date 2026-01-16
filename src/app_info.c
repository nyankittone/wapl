// This file defines functions for handling not just the AppInfo struct, but also color pallettes!

#include "wapl.h"

// A limitation with the approach I'm using here is that I can't easily compose highlights out of
// other highlights as the caller. I don't think this is the end of the world, since it's still
// possible with sprintf/snprintf, and it feels like a niche case. However having the ability to
// override the default highlight value in certain situations could actually be really handy for
// making certain kind of messages easier to display.
static wapl_Highlights default_highlights[] = {
    [WAPL_HL_DEFAULT] = "",
    [WAPL_HL_BOLD] = "\33[1m",
    [WAPL_HL_ITALIC] = "\33[3m",
    [WAPL_HL_UNDERLINE] = "\33[4m",
    [WAPL_HL_BLINK] = "\33[5m",
    [WAPL_HL_INFO] = "\33[1m",
    [WAPL_HL_WARN] = "\33[1;93m",
    [WAPL_HL_ERROR] = "\33[1;91m",
    [WAPL_HL_FATAL] = "\33[1;91m",
    [WAPL_HL_NAME] = "fart",
    [WAPL_HL_AUTHOR] = "fart",
    [WAPL_HL_VERSION] = "fart",
    [WAPL_HL_URL] = "fart",
};

