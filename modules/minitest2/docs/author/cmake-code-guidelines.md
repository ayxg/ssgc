# CMake Code Guidelines

## Writing Functions/Macros
- cmake_parse_arguments: set prefix to "arg"
- All cmake_parse_arguments must use PARSE_ARGV.
- Validate all required arguments, warn on detected unparsed arguments.

## Naming variables
- Don't use reserved CMake identifers, which :
    - begin with CMAKE_ (upper-, lower-, or mixed-case), or
    - begin with _CMAKE_ (upper-, lower-, or mixed-case), or
    - begin with _ followed by the name of any CMake Command.
- Functions and macros use **snake_case** prefixed with **[module-prefix]_**.
- Local variables are named with **snake_case**
- Public global variables are **ALL_CAPS** prefixed with **[module-prefix]_**.
- Internal global variable are **ALL_CAPS** prefixed with **Z_[module-prefix]_**.
- External experimental global variable names are **ALL_CAPS** prefixed with **X_[module-prefix]_**.
- Internal functions are prefixed with **z_[module-prefix]_**
- Functions which are internal to a single function (i.e., helper functions) are named [z_]<func>_<name>, 
where <func> is the name of the function they are a helper to, and <name> is what the helper function does.
z_ should be added to the front if <func> doesn't have a z_, but don't name a helper function z_z_foo_bar.
