#include <iostream>

#include "Command.hpp"
#include "Executor.hpp"

int main(int argc, char *argv[])
{

    {
        std::cout << "------ Start 'ls ../src -a > test' command ------\n";

        char *ls_args[] = {"ls", "../src", "-a", NULL};
        command_t ls = {"/bin/ls", ls_args, NULL, "./test"};
        Executor executor = {ls};
        executor.exec();

        std::cout << "------ End command ------\n\n";
    }

    {
        std::cout << "------ Start 'cat test' command ------\n";

        char *cat_args[] = {"cat", "test", NULL};
        command_t cat = {"/bin/cat", cat_args, NULL, NULL};
        Executor executor = {cat};
        executor.exec();

        std::cout << "------ End command ------\n\n";
    }

    {
        std::cout << "------ Start 'grep .cpp < test' command ------\n";

        char *grep_args[] = {"grep", ".cpp", NULL};
        command_t grep = {"/bin/grep", grep_args, "./test", NULL};
        Executor executor = {grep};
        executor.exec();

        std::cout << "------ End command ------\n\n";
    }

    {
        std::cout << "------ Start 'grep .cpp < test | grep main' command ------\n";

        char *grep1_args[] = {"grep", ".cpp", NULL};
        command_t grep1 = {"/bin/grep", grep1_args, "./test", NULL};
        char *grep2_args[] = {"grep", "main", NULL};
        command_t grep2 = {"/bin/grep", grep2_args, NULL, NULL};
        Executor executor = {grep1, grep2};
        executor.exec();

        std::cout << "------ End command ------\n\n";
    }

    {
        std::cout << "------ Start 'ls ../src/ -a | grep .cpp | grep main | sed s/main/Success/' command ------\n";

        char *ls_args[] = {"ls", "../src", "-a", NULL};
        command_t ls = {"/bin/ls", ls_args, NULL, NULL};
        char *grep1_args[] = {"grep", ".cpp", NULL};
        command_t grep1 = {"/bin/grep", grep1_args, NULL, NULL};
        char *grep2_args[] = {"grep", "main", NULL};
        command_t grep2 = {"/bin/grep", grep2_args, NULL, NULL};
        char *sed_args[] = {"sed", "s/main/Success/", NULL};
        command_t sed = {"/bin/sed", sed_args, NULL, NULL};
        Executor executor = {ls, grep1, grep2, sed};
        executor.exec();

        std::cout << "------ End command ------\n\n";
    }

    return 0;
}
