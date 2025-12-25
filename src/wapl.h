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
 */

#endif
