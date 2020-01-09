#ifndef BASH_EXECUTOR_HPP
#define BASH_EXECUTOR_HPP

#include <vector>
#include <memory>
#include <initializer_list>

#include "Command.hpp"

struct command_t {
    char*  path;
    char** args;
    char*  input;
    char*  output;
};

using commands_t = std::vector<command_t>;

class Executor {
public:
    Executor(const std::initializer_list<command_t>&);

    int exec();

private:
    commands_t commands_;
    bool isLastCommand(const commands_t::iterator&);
};

#endif