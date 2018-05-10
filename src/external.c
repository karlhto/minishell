#include "external.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*! \fn     fork_proc()
 * The first argument is a vector of char pointers, pointing to the beginning
 * of the program name at position 0, and the arguments of the program in the
 * subsequent vectors. Can specify
 *
 * @param	char **args
 */
pid_t fork_proc(char **args, int in, int out) {
    if (!args) {
        fprintf(stderr, "fork_proc(): args cannot be empty\n");
        return -1;
    }

    pid_t cpid = fork();
    if (cpid == 0) {
        if (in > 0) {
            if (dup2(in, STDIN_FILENO)) {
                perror("dup2()");
                exit(EXIT_FAILURE);
            }

            close(in);
        }

        if (out > 0) {
            if (dup2(out, STDOUT_FILENO)) {
                perror("dup2()");
                exit(EXIT_FAILURE);
            }

            close(out);
        }

        if (execvp(args[0], args)) {
            perror("execvp()");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    } else if (cpid < 0) {
        perror("minishell:");
        return -1;
    }

    DLOG("fork_proc(): starting new process with id %d: %s\n", cpid, args[0]);
    return cpid;
}
