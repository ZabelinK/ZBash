#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdexcept>

#include "Executor.hpp"
#include "Command.hpp"

Executor::Executor(const std::initializer_list<command_t>& commands) :
    commands_(commands.begin(), commands.end())
{}

int Executor::exec()
{
    _exec_next(commands_.begin());
}

int Executor::_exec_next(commands_t::iterator next_cmd)
{
    int file, chield_pid, pipefd[2], curr_input;

    if (next_cmd == commands_.end()) {
        return 0;
    }

    switch (next_cmd->type)
    {
    case LT_COMMAND:
        if ((chield_pid = fork()) != 0) {
            return chield_pid;
        }
        break;
    case LT_IN_REDIRECT:
        if ((file = open(next_cmd->filename, O_RDONLY)) < 0) {
            throw std::runtime_error("Can't open file " + next_cmd->filename);
        }

        dup2(STDIN_FILENO, file);
        close(file);
        break;
    case LT_OUT_REDIRECT:
        if ((file = open(next_cmd->filename, O_WRONLY | O_CREAT | O_TRUNC)) < 0) {
            throw std::runtime_error("Can't open file " + next_cmd->filename);
        }
        dup2(STDOUT_FILENO, file);
        close(file);
        break;

    case LT_PIPE:
        if (pipe(pipefd) < 0) {
            throw std::runtime_error("Can't open pipe");
        }

        curr_input = dup(STDIN_FILENO);
        dup2(STDIN_FILENO, pipefd[1]);
        close(pipefd[1]);
        break;
    default:
        throw std::runtime_error("Invalid cmd type");
    }

    _exec_next(next_cmd + 1);

    switch (next_cmd->type)
    {
    case LT_COMMAND:
        execv(next_cmd->command, next_cmd->args);
        break;
    case LT_IN_REDIRECT:
    case LT_OUT_REDIRECT:
        break;

    case LT_PIPE:
        dup2(STDIN_FILENO, curr_input);
        close(curr_input);
        dup2(STDOUT_FILENO, pipefd[0]);
        close(pipefd[0]);
        break;
    default:
        throw std::runtime_error("Invalid cmd type");
    }

    return 0;
}
