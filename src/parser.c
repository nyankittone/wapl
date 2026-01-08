#include "wapl.h"

#define POSITIONAL_ARGS_MAX (16)
#define SUBCOMMAND_MAX (16)
#define OPTIONAL_ARGS_MAX (32)

typedef struct Parser {
    char *short_description;
    char *long_description; /* TODO: See if a more sophisticated string type would be better
                                     here. */

    struct {
        size_t count;
        wapl_PositionalArg array[POSITIONAL_ARGS_MAX];

        char **variable_args; // I think it makes sense to let the arg parser manage the variadic
                              // positionals.
    } positionals;

    struct {
        size_t count;
        wapl_OptionalArg array[OPTIONAL_ARGS_MAX]; /* Will have to think about memory management soon */
        wapl_OptionalArg *ptr;
        size_t capacity;

        /* Here we'll probably want some logic for controlling how the help text is printed out. */        
        wapl_OptionalArg help;
        wapl_OptionalArg version;
    } options;

    struct {
        /* I do not like this way of allocating memory for the subcommands. I should make a fancy
         * allocator for this stuff. */
        struct Parser *array;
        size_t count;
        size_t capacity;
    } subcommands;
} Parser;

