/*! \file   shell.c
 * Has main event loop.
 */
#include "builtin.h"
#include "external.h"
#include "shell.h"
#include "strutil.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __linux__
#include <bits/sigaction.h>
#endif

static void sigact(int signo);

static volatile int stop = 0;

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
    struct sigaction sa;

    sa.sa_handler = sigact;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction()");
        return -1;
    }

    if (init_builtin() == BUILTIN_ERROR)
        return -1;

    char line[256];
    while (!stop) {
        pwd_builtin();

        if (fgets(line, sizeof(line), stdin)) {
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
                        stop = !stop;
                        break;
                    case BUILTIN_EXIT:
                        DLOG("Exiting program, %d\n", ret);
                        stop = !stop;
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
    }
}
