#include "Command.hpp"
#include "Executor.hpp"

int main(int argc, char *argv[])
{
    lexem_t ls = {.type = LT_COMMAND, .command = "ls", .args = {NULL} };
    lexem_t out = {.type = LT_OUT_REDIRECT, .filename = "out"};
    Executor executor = {ls, out};
    executor.exec();

    return 0;
}
