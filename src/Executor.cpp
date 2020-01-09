#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h> 
#include <fcntl.h>

#include <stdexcept>
#include <iostream>

#include "Executor.hpp"
#include "Command.hpp"

Executor::Executor(const std::initializer_list<command_t>& commands) :
    commands_(commands.begin(), commands.end())
{}

bool Executor::isLastCommand(const commands_t::iterator& cmd)
{
    return (cmd + 1) == commands_.end();
}

int Executor::exec()
{
    int rc = 0;
    int input_pipe[2] = {-1, -1}, output_pipe[2] = {-1, -1};
    unsigned int chield_cnt = 0;

    for (commands_t::iterator command = commands_.begin();
         command != commands_.end();
         ++command) {

        close(input_pipe[0]);
        close(input_pipe[1]);
        input_pipe[0] = output_pipe[0];
        input_pipe[1] = output_pipe[1];
        output_pipe[0] = -1;
        output_pipe[1] = -1;

        if (!isLastCommand(command)) {
            if (pipe(output_pipe) < 0) {
                std::cerr << "Can't create pipe errno - " << errno << '\n';
                exit(1);
            }
        }

        if ((rc = fork()) == 0) {
            int in_fd, out_fd;
            if (input_pipe[0] != -1) {
                dup2(input_pipe[0], STDIN_FILENO);
                close(input_pipe[0]);
                close(input_pipe[1]);
            }

            if (output_pipe[0] != -1) {
                dup2(output_pipe[1], STDOUT_FILENO);
                close(output_pipe[0]);
                close(output_pipe[1]);
            }

            if (command->input) {
                int file_fd = open(command->input, O_RDONLY);
                if (file_fd < 0) {
                    std::cerr << "Can't open file on read, errno - " << errno << '\n';
                    exit(1);
                }
                dup2(file_fd, STDIN_FILENO);
                close(file_fd);
            }

            if (command->output) {
                int file_fd = open(command->output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (file_fd < 0) {
                    std::cerr << "Can't open/create file on write, errno - " << errno << '\n';
                    exit(1);
                }
                dup2(file_fd, STDOUT_FILENO);
                close(file_fd);
            }

            execv(command->path, command->args);
        } else if (rc < 0) {
            std::cerr << "Can't create chield proccess, errno - " << errno << '\n';
            exit(1);
        }

        ++chield_cnt;
    }

    close(input_pipe[0]);
    close(input_pipe[1]);
    close(output_pipe[0]);
    close(output_pipe[1]);

    while (chield_cnt-- > 0) {
        wait(NULL);
    }
}