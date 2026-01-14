#ifndef _WAPL_DEFINED
#define _WAPL_DEFINED

#include <stdbool.h>

/* Okay, so how will I design this library?
 * For color pallettes, I want them to be stored in "color contexts", where an object can be used
 * with a specific color context in mind. There should also be a "global" color context that exists
 * by default and can be written to.
 * In fact, it may be a good idea to have "contexts" for a variety of assets that could change.
 * We'll also want some global state for the program name, its version number, and possibly other
 * details I forgot. This should be stuff that will be used to initialize the argument parser
 * object. We should also make it in such a way so it can be easily extended with custom info.
 *
 * The argument parser needs to support a handful of features for me to be happy:
 * positional arguments
 * both long and short options, according to POSIX convention
 * "--" option to disable processing of further options
 * Automatic handling of --help and --version options - but provide a way to not handle those
 * Support optional parameters for options, and having more than one parameter!
 * Support subcommands, each of which is built from the same struct as a main command.
 * Having the ability to compose multiple commands together from other commands. This could be
 * implemented with a list of poinbters to other commands.
 * 
 * It might also be cool to have a system in place for suggestions on misspelled options and
 * subcommands, but that may be more overhead than I'd like for some of the places this library may
 * be used in. If I implement this, it shouldf be optional and disabled by default.
 * It's a good idea to provide a way to have the library consumer cleanly integrate
 * such a service into the argument parser if they so choose.
 *
 * There should be routines here for my own custom asserts; ones that will produce fancier error
 * messages on assersion failure. This would also be associated with a system for panicing. These
 * routines should use the context system that all the other functions use.
 *
 * We should probably have 2 sets of functions: one that uses the global context, and one that
 * uses whatever context is fed in as a parameter.
 *
 * Strings passed into this library should be length-prefixed and have a lifetime for the whole
 * program, i.e. a string literal.
 *
 * I want to have the ability to define custom colors and text strings for special needs; how should
 * these custom values be referenced? By string in some kind of table? or as a raw integer that
 * looks up into an array? I think the latter is more attractive.
 *
 * Required options from clap is a good feature idea that I should totally implement into this
 * library. I also want it to take form of a builder pattern for creating the parser. Also, when
 * building the parser, we should specify the positional arguments too, so that the parser can be
 * aware of what the positional args are supposed to be and thus can show that information in the
 * help printout.
 *
 * Clap supports the idea of variadic parameters for options; I don't really like that idea, so I
 * don't think I'll implement it. There could however be a use having a custom tokenizing syntax for
 * specific options. I don't know if I want to implement this, though. It will probably be lower on
 * my implementation priority.
 */

#include <stddef.h>

/* How will I "extend" this to include custom fields other than ones in there? I should read over
 * my brainstorming at some point... */

// So, I kinda want this library to be a little more thread-safe, so in the bindings, I don't have
// to slap a mutex on all of the global data.
// The simple and most portable way to do this is by not having the library's globals be mutable at
// all. They're only readable, and even then, indirectly through a function interface. We configure
// stuff not by mutating global state, but instead by getting some template object and changing that
// locally, before passing that object to something else. This is OK, but it means I have no control
// over the "default" properties spit out by those functions. This is mainly an issue with the
// color pallette object.
// Another possibility would be to have a way to configure global state in one shot, and have a lock
// to prevent changes to the configuration after the fact, but this one-shot approach can't be
// thread-safe as long as the value used to lock is not atomic, which we can olny get with either
// C11 or non-portable methods. So the former approach is the way to go here.

#define WAPL_STRING_COLLECTION_CAPACITY (256)
typedef struct {
    char *array[WAPL_STRING_COLLECTION_CAPACITY];
    char **extra;
    size_t largest_index;
    size_t capacity;
} wapl_StringCollection;

// Defining index values for highlighting rules. A better way to do this would be to generate code
#define WAPL_HL_DEFAULT ((size_t) 1)
#define WAPL_HL_INFO ((size_t) 2)
#define WAPL_HL_WARN ((size_t) 3)
#define WAPL_HL_ERROR ((size_t) 4)
#define WAPL_HL_FATAL ((size_t) 5)
#define WAPL_HL_NAME ((size_t) 6)
#define WAPL_HL_AUTHOR ((size_t) 7)
#define WAPL_HL_VERSION ((size_t) 8)
#define WAPL_HL_URL ((size_t) 9)
#define WAPL_HL_BEYOND_DEFAULT ((size_t) 10)
typedef wapl_StringCollection wapl_Highlights;

#define WAPL_APPINFO_NAME ((size_t) 1)
#define WAPL_APPINFO_AUTHOR ((size_t) 2)
#define WAPL_APPINFO_AUTHOR_PRETTY ((size_t) 3)
#define WAPL_APPINFO_VERSION ((size_t) 4)
#define WAPL_APPINFO_URL ((size_t) 5)
#define WAPL_APPINFO_SHORT_DESCRIPTION ((size_t) 6)
#define WAPL_APPINFO_LONG_DESCRIPTION ((size_t) 7)
#define WAPL_APPINFO_BEYOND_DEFAULT ((size_t) 8)
typedef wapl_StringCollection wapl_AppInfo;

// TODO: Anything in a Context struct should have a way to add cutom fields. How tf will I do that?
// typedef struct {
//     char *name;
//     char *author;
//     char *version;
//     char *url;
//     char *short_description;
//     char *long_description;
// } wapl_AppInfo;

// The context object is passed to things like the logger and pargument parser, and similar things.
// It specifies things should be printed out, and what should be printed when we need to print, for
// example, the application name.
// For most cases, when we copy a context, we want the color pallette to be kept, so a pointer seems
// ideal for this. TODO: Think about how the logger would use this context effectively.
typedef struct {
    wapl_Highlights *colors;
    wapl_AppInfo app_info;
} wapl_Context;

// The `mask` parameter here and for wapl_makeApp is used to "customize" the default color pallette
// and app info returned. Any fields in those mask parameters that are non-zero or non-NULL will
// "overwrite" the default value for that field.
wapl_Highlights wapl_makePallette(wapl_CoreHighlights mask);
wapl_Context wapl_makeApp(wapl_ColorPallette *const colors, wapl_AppInfo app_mask);

// TODO: Having the library hand an opaque pointer for a compound error while handling allocation
// behind the scenes is not thread-safe. We'll need to actually have the type definition for a
// compound error here so we can return it by-value to the caller. That means I need to decide
// how this thing is composed...
typedef void *wapl_CompoundError;

// TODO: I would prefer having this thing return a CompoundError that is then merged into a greater
// one, but that may have implications on performance. If I can find a way to address this, I want
// to change the CompoundError being passed in via a parameter to a thing returned.
typedef void (*wapl_Converter)(wapl_CompoundError *const errors, char *const param, size_t param_length, void *const target);
#define mConverter(name) void name \
    (wapl_CompoundError *const errors, char *const param, size_t param_length, void *const target)

// A "type" in this library is some info used to specify how some kinds of parameters should be shown in the help, and how they should be converted from their string form to whatever.
typedef struct {
    char *name;
    char *highlighting;
    wapl_Converter converter; /* How do I in Rust make a safe interface for casting the void*? */
} wapl_Type;

wapl_Type wapl_copyType(const wapl_Type *const type); // I forgot why this function exists.
wapl_Type wapl_deriveType(const wapl_Type *const type, wapl_Converter custom_converter); // idk...

// Providing some pre-defined types sqo that parsing basic parameters is easier to do.
extern const wapl_Type wapl_type_c_int;
extern const wapl_Type wapl_type_c_long;
extern const wapl_Type wapl_type_c_short;
extern const wapl_Type wapl_type_c_char;
extern const wapl_Type wapl_type_i8;
extern const wapl_Type wapl_type_i16;
extern const wapl_Type wapl_type_i32;
extern const wapl_Type wapl_type_i64;
extern const wapl_Type wapl_type_isize;
extern const wapl_Type wapl_type_u8;
extern const wapl_Type wapl_type_u16;
extern const wapl_Type wapl_type_u32;
extern const wapl_Type wapl_type_u64;
extern const wapl_Type wapl_type_usize;
extern const wapl_Type wapl_type_f32;
extern const wapl_Type wapl_type_f64;
extern const wapl_Type wapl_type_string;

typedef struct {
    char *label;
    wapl_Type type;
} wapl_ParameterSpec;

typedef struct {
    wapl_ParameterSpec *const ptr;
    size_t length;
} wapl_ParameterSpecs;

// An action is a function that is used to actually write the parsed data into the destination
// structure. When attached to a parser, it can be set up to either target one field in a struct, or
// the struct as a whole if the action was programmed to treat the target pointer as that kind of
// struct. The latter case is useful when you have a more complicated action that needs more context
// than just the destination of one data point. The disadvantage of this approach is that it's not
// at all type-safe... I do wonder how we'll handle this in a nicer way within Rust, if we can.
typedef wapl_CompoundError (*wapl_Action)(wapl_ParameterSpecs params, void *const target);

// Some pre-defined actions, so that you won't have to write your own in the common case.
extern const wapl_Action wapl_action_c_int;
extern const wapl_Action wapl_action_c_long;
extern const wapl_Action wapl_action_c_short;
extern const wapl_Action wapl_action_c_char;
extern const wapl_Action wapl_action_f32;
extern const wapl_Action wapl_action_f64;
extern const wapl_Action wapl_action_true;
extern const wapl_Action wapl_action_false;
extern const wapl_Action wapl_action_i8;
extern const wapl_Action wapl_action_i16;
extern const wapl_Action wapl_action_i32;
extern const wapl_Action wapl_action_i64;
extern const wapl_Action wapl_action_isize;
extern const wapl_Action wapl_action_u8;
extern const wapl_Action wapl_action_u16;
extern const wapl_Action wapl_action_u32;
extern const wapl_Action wapl_action_u64;
extern const wapl_Action wapl_action_usize;
extern const wapl_Action wapl_action_string;

typedef struct {
    char **ptr;
    size_t length;
} wapl_CStrings;

typedef struct {
    wapl_Type *const ptr;
    size_t length;
} wapl_Types;

// This type is used for passing boolean flags to things in this library. I just wanted a name for
// it that's more descriptive then `int`.
typedef int wapl_OptionFlag;

typedef struct {
    char *description;
    wapl_ParameterSpecs params;
    wapl_Action action;
    void *const target;
} wapl_PositionalArg;

typedef struct {
    char *shorthands;
    wapl_CStrings longhands;
    char *description;
    wapl_ParameterSpecs params; /* I will want generator functions for setting up actions and types
                                   for single-parameter options. */
    wapl_Action action;
    void *const target;
    wapl_OptionFlag flags;
} wapl_OptionalArg;

typedef struct {
    char *small;
    char *large;
} wapl_CmdDescription;

/* Argument parser. */
typedef void *wapl_Parser;
wapl_Parser wapl_newParser(const char *const name);
void wapl_addParserPositionals(wapl_Parser *const parser, const wapl_PositionalArg *const args, const size_t args_length);
void wapl_addParserOptionals(wapl_Parser *const parser, const wapl_OptionalArg *const args, const size_t args_length);

void wapl_addParserDesc(wapl_Parser *const parser, const wapl_CmdDescription desc);

/* NOTE: This signature is up for change. I may make it only return a vector of positional
 * arguments instead of an error, and will just try to error out with a compound error it holds in
 * its private state. I can only do this if I enforce that extra types can be used to validate the
 * entire input. */
char **wapl_parse(wapl_Parser *const parser);

typedef short wapl_Enum;
wapl_Parser wapl_addSubcommand(wapl_Parser *const parent, const char *const name, wapl_Enum flag, wapl_Enum *const flag_target);

// We can customize an argument parser's interpretation of options (and only options) on the level
// of individual ones. Basically controlling the syntax of the start of a flag and what splits
// individual parameters for that option.
// To keep things simple and in my bounds of sanity, I think I'll only have global parser rules for
// this. We need:
// * A function for looking at a token to see if the thing we're on is the start of a short or long
// option or neither
// * A boolean for controlling if I want short options to be combinable in a single token (there
// might be a more flexiable way of doing this that I can't think of, but this is simple and likely
// good enough for my needs)
// * A function for determining what splits the beginning of an option specifier with its
// parameters. Should be possible to have no split, and have it controllable based on the option in
// question.
// * Either a list of delimiters or a function for determining how to split parameters into
// individual tokens. The function way is more flexiable, so I'll probably go with that.
typedef struct {
    enum {
        WAPL_TOKEN_TYPE_POSITIONAL,
        WAPL_TOKEN_TYPE_SHORT,
        WAPL_TOKEN_TYPE_LONG,
    } kind;
    char *option_name; // parameter is ignored if `kind` is `WAPL_TOKEN_TYPE_POSITIONAL`. 
    size_t option_name_length; // parameter is ignored if `kind` is `WAPL_TOKEN_TYPE_POSITIONAL`.
    char *remaining; // parameter is ignored if `kind` is `WAPL_TOKEN_TYPE_POSITIONAL`.
} wapl_TokenInfo;

typedef struct {
    bool has_first;
    char *first;
} wapl_FirstArgResult;

typedef struct {
    bool split_it;
    char *left_split;
    size_t left_split_length;
    char *split_remaining;
} wapl_TokenSplitResult;

typedef struct {
    wapl_TokenInfo (*token_type_finder)(char *token);
    bool shorts_combinable;
    wapl_FirstArgResult (*find_parameters_start)(size_t tokens_scanned, const char *token);
    wapl_TokenSplitResult (*token_splitter)(size_t tokens_scanned, const char *token);
} wapl_OptionParser;

// TODO: Don't forget required options!
// TODO: Add a way to set up an argument parser that can persist between multiple invocations
// without knowing until parse time what struct it needs to write to. I think the simplest way is to
// make a way for it to store a pointer offset into a struct, rather than a raw pointer that looks
// at a field of an already defined struct.
// Except wait a minute, how would subcommands work? With this model, subcommand structs must be
// stored inside the root command struct. Is this really how I want to structure my data?
// I think I *am* okay with it overall, because otherwise I would not know how to do this lmao

#endif
