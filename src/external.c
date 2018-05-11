/*! \file   external.c
 * Contains external commands.
 */
#include "external.h"
#include "strutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __linux__
#include <sys/wait.h>
#endif

#define READ_END 0
#define WRITE_END 1

/*! \fn pipe_procs
 * \brief Runs executables and pipes if pipes were found.
 *
 * Creates pipes if found, otherwise just starts a program. Pipes are created
 * according to number of processes - 1, and are then passed on to the
 * respective programs. If processes are to be run in the background, sends
 */
int pipe_procs(char ***procs, int background) {
    if (!procs)
        return 0;

    int num_procs = 0;
    for (int i = 0; procs[i]; i++)
        num_procs++;

    int num_pipes = num_procs - 1;
    int proc_pids[num_procs];

    if (num_pipes > 0) {
        int pipes[num_pipes][2];
        for (int i = 0; i < num_pipes; i++) {
            if (pipe(pipes[i])) {
                perror("pipe()");
                return -1;
            }
        }

        proc_pids[0] = fork_proc(procs[0], 0, pipes[0][WRITE_END]);
        close(pipes[0][WRITE_END]);

        for (int i = 1; i < num_procs - 1; i++) {
            proc_pids[i] = fork_proc(procs[i], pipes[i - 1][READ_END],
                                     pipes[i][WRITE_END]);
            close(pipes[i - 1][READ_END]);
            close(pipes[i][WRITE_END]);
        }

        int n = num_procs - 1;
        int m = num_pipes - 1;
        proc_pids[n] = fork_proc(procs[n], pipes[m][READ_END], 0);
        close(pipes[m][READ_END]);

        if (!background) {
            for (int i = 0; i < num_procs; i++)
                waitpid(proc_pids[i], NULL, 0);
        } else {
            printf("Started processes in background: ");
            for (int i = 0; i < num_procs - 1; i++)
                printf("%d ", proc_pids[i]);

            printf("%d\n", proc_pids[num_procs - 1]);
        }
    } else {
        proc_pids[0] = fork_proc(procs[0], 0, 0);
        if (proc_pids[0] < 0)
            return -1;

        int status = 0;

        if (!background)
            waitpid(proc_pids[0], &status, 0);
        else
            printf("Started process in background: %d\n", proc_pids[0]);
    }

    return 0;
}

/*! \fn fork_proc
 * \brief
 * The first argument is a vector of char pointers, pointing to the beginning
 * of the program name at position 0, and the arguments of the program in the
 * subsequent vectors. Can specify in and out filedescriptors for either pipes
 * or redirects.
 *
 * Returns the client PID.
 */
pid_t fork_proc(char **args, int in, int out) {
    if (!args) {
        fprintf(stderr, "fork_proc(): args cannot be empty\n");
        return -1;
    }

    pid_t cpid = fork();
    if (cpid == 0) {
        if (in > 0) {
            if (dup2(in, STDIN_FILENO) < 0) {
                perror("dup2()");
                exit(EXIT_FAILURE);
            }

            close(in);
        }

        if (out > 0) {
            if (dup2(out, STDOUT_FILENO) < 0) {
                perror("dup2()");
                exit(EXIT_FAILURE);
            }

            close(out);
        }

        if (execvp(args[0], args)) {
            perror("execvp()");
            exit(EXIT_FAILURE);
        }

        free(args);
        exit(EXIT_SUCCESS);
    } else if (cpid < 0) {
        perror("minishell:");
        return -1;
    }

    DLOG("Starting new process with id %d: %s\n", cpid, args[0]);
    return cpid;
}
