#ifndef BASH_COMMAND_HPP
#define BASH_COMMAND_HPP

#include <string>
#include <memory>

enum LEXEM_TYPE {
    LT_COMMAND,
    LT_IN_REDIRECT,
    LT_OUT_REDIRECT,
    LT_PIPE,
    LT_OUT_OF,
};

struct lexem_t {
    LEXEM_TYPE type;
    union {
        struct {
            char *command;
            char *args[];
        };
        struct {
            char *filename;
        };
    };
};

#endif