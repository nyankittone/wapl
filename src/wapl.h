#ifndef _WAPL_DEFINED
#define _WAPL_DEFINED

/* Okay, so how will I design this library?
 * I want to make it so that objects are handled through opaque pointers haded back from a factory
 * function. This will make implementing the Rust binding easier to do, and makes it harder for
 * calling code to break invariants.
 * Doing the above will make it not really possible to have objects live on the stack, so we'll need
 * a fast way of creating and destroying objects, and functions for destroying any given object.
 * For color pallettes, I want them to be stored in "color contexts", where an object can be used
 * with a specific color context in mind. There should also be a "global" color context that exists
 * by default and can be written to.
 * In fact, it may be a good idea to have "contexts" for a variety of assets that could change, as
 * well as a "meta-context" that can be composed of multiple other contexts.
 * We'll also want some global state for the program name, its version number, and possibly other
 * details I forgot. This should be stuff that will be used to initialize the argument parser
 * object.
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

/* Functions for setting the app name, the version, the author, the project URL, and other information.
 * TODO: I think it would be nice if we had a way to add custom global metadata as well. I should add
 * something for this. */
void wapl_setVersion(const char *const version, const size_t length);
void wapl_setAuthor(const char *const author, const size_t length);
void wapl_setUrl(const char *const url, const size_t length);

/* Context object definition. Contexts have their whole struct passed directly back to the caller,
 * instead of being accessed through a pointer, since we're OK with the client having full
 * read-write access to individual fields in the struct. */
typedef struct {
    const char* ptr;
    size_t length;
} wapl_String;

/* Convenience for C consumers of the library so they can alias a C-style string into a context or
 * something similar. The "C" notation here indicated that this function is meant to be called
 * from C code only. */
void wapl_C_setString(wapl_String *const dest, const char *const src);

typedef struct {
    wapl_String color_info,
                color_warn,
                color_error,
                color_fatal;
} wapl_Context;

wapl_Context wapl_copyContext(const wapl_Context *const context);

typedef void *const wapl_CompoundError;
typedef wapl_CompoundError (*wapl_Converter)(wapl_String in, void *const out);

typedef struct {
    wapl_String name;
    wapl_String highlighting;
    wapl_Converter converter; /* How I in Rust make a safe interface for casting the void*? */
} wapl_Type;

wapl_Type wapl_copyType(const wapl_Type *const type);
wapl_Type wapl_copyTypeWithConverter(const wapl_Type *const type, wapl_Converter converter);
extern const wapl_Type *const wapl_type_i8;
extern const wapl_Type *const wapl_type_i16;
extern const wapl_Type *const wapl_type_i32;
extern const wapl_Type *const wapl_type_i64;
extern const wapl_Type *const wapl_type_isize;
extern const wapl_Type *const wapl_type_u8;
extern const wapl_Type *const wapl_type_u16;
extern const wapl_Type *const wapl_type_u32;
extern const wapl_Type *const wapl_type_u64;
extern const wapl_Type *const wapl_type_usize;
extern const wapl_Type *const wapl_type_f32;
extern const wapl_Type *const wapl_type_f64;
extern const wapl_Type *const wapl_type_make_true;
extern const wapl_Type *const wapl_type_make_false;
extern const wapl_Type *const wapl_type_string;

typedef struct {
    wapl_String *const ptr;
    size_t length;
} wapl_StringCollection;

/* Does this type here make sense to be a part of this header file? */
typedef struct {
    wapl_Type *const ptr;
    size_t length;
} wapl_TypeCollection;

typedef struct {
    wapl_String label;
    wapl_String description;
    wapl_Type type;
} wapl_PositionalArg;

typedef struct {
    wapl_String shorthands;
    wapl_StringCollection longhands;
    wapl_String description;
    wapl_TypeCollection parameters;
} wapl_OptionalArg;

typedef struct {
    wapl_String small;
    wapl_String large;
} wapl_CmdDescription;

/* Argument parser. */
typedef void *const wapl_Parser;
wapl_Parser wapl_newParser(const char *const name, const size_t name_length);
void wapl_addParserPositionals(wapl_Parser *const parser, const wapl_PositionalArg *const args, const size_t args_length);
void wapl_addParserOptionals(wapl_Parser *const parser, const wapl_OptionalArg *const args, const size_t args_length);

void wapl_addParserDesc(wapl_Parser *const parser, const wapl_CmdDescription desc);

/* NOTE: This signature is up for change. I may make it only return a vector of positional
 * arguments instead of an error, and will just try to error out with a compound error it holds in
 * its private state. I can only do this if I enforce that extra types can be used to validate the
 * entire input. */
wapl_StringCollection wapl_parse(wapl_Parser *const parser);

wapl_Parser wapl_addSubcommand(wapl_Parser *const parent, const char *const name, const size_t name_length);

#endif
