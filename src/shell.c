/*! \file   shell.c
 * Has main event loop.
 */
#ifdef __linux__
#define _POSIX_SOURCE
#endif

#include "builtin.h"
#include "external.h"
#include "shell.h"
#include "strutil.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void sigact(int signo);

volatile sig_atomic_t stop;

/*! \fn run_shell
 * \brief Contains event loop of shell.
 *
 * Initialises builtin commands and signals, then enters main loop where it
 * waits for input, parses it (by pipes first, then by spaces (with parse_str).
 * It then looks through builtin commands, and if none are found, program(s)
 * is/are run.
 *
 * Returns 0 on a successful exit, -1 if anything goes wrong.
 */
int run_shell() {
    int ret = 0;
    stop = 0;

    struct sigaction sa;
    sa.sa_handler = sigact;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction()");
        return -1;
    }

    // will be replaced eventually, is supposed to kill active non-background
    // children
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction()");
        return -1;
    }

    if (init_builtin() == BUILTIN_ERROR)
        return -1;

    char line[256];
    while (!stop) {
        pwd_builtin();

        char *ptr = fgets(line, sizeof(line), stdin);
        if (!ptr) {
            stop = 1;
            if (errno != 0) {
                perror("fgets()");
                ret = -1;
            }
        } else {
            // TODO find a better solution to this, looks unintuitive
            int background = 1;

            while (!stop && background) {
                char ***procs = str_parse(line, &background);
                if (!procs) {
                    ret = -1;
                    break;
                }

                int ret = exec_builtin(procs[0]);
                switch (ret) {
                    case BUILTIN_NOT_FOUND:
                        DLOG("Executing external command, %d\n", ret);
                        pipe_procs(procs, background);
                        break;
                    case BUILTIN_ERROR:
                        DLOG("Error encountered during builtin command, %d\n",
                                ret);
                        ret = -1;
                        stop = 1;
                        break;
                    case BUILTIN_EXIT:
                        DLOG("Exiting program, %d\n", ret);
                        stop = 1;
                        break;
                    case BUILTIN_SUCCESS:
                        DLOG("Succesfully executed builtin command, %d\n",
                                ret);
                        break;
                    default:
                        fprintf(stderr,
                                "run_shell(): unsupported option: %d\n", ret);
                        break;
                }

                for (int i = 0; procs[i]; i++)
                    free(procs[i]);
                free(procs);
            }
        }
    }

    free_builtin();
    return ret;
}

/*! \fn sigact
 * Handles signals.
 *
 * More to come, eventually.
 */
static void sigact(int signo) {
    switch (signo) {
        case SIGTERM:
            stop = 1;
            break;
        case SIGINT:
            stop = 1;
            break;
    }
}
