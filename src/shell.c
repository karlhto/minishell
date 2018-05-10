/**
 *
 */
#include "shell.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//static int run_proc(char **args, int in_pipe);
static int fork_proc(char **args, int in, int out);

int run_shell() {
    char line[256];
    while (1) {
        if (fgets(line, sizeof(line), stdin)) {
            char **strlst = strsplt_ws(line);
            if (!strlst)
                continue;

            pid_t cpid = fork_proc(strlst, 0, 0);
            free(strlst);
            int status = 0;
            waitpid(cpid, &status, 0);
        }
    }

    return 0;
}

/**
 * Function for forking processes.
 * The first argument is a vector of char pointers, pointing to the beginning
 * of the program name at position 0, and the arguments of the program in the
 * subsequent vectors. Can specify
 *
 * @param	char **args
 */
static pid_t fork_proc(char **args, int in, int out) {
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
