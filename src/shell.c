/*! \file   shell.c
 *
 */
#include "builtin.h"
#include "external.h"
#include "shell.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
