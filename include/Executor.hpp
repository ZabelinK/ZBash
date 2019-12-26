#ifndef BASH_EXECUTOR_HPP
#define BASH_EXECUTOR_HPP

#include <vector>
#include <memory>
#include <initializer_list>

#include "Command.hpp"

using command_t = lexem_t;
using commands_t = std::vector<command_t>;

class Executor {
public:
    Executor(const std::initializer_list<command_t>&);

    int exec();

private:
    int _exec_next(commands_t::iterator);
    commands_t commands_;
};

#endif