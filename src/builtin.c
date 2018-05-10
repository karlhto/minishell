#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int run_builtin(char **args) {
    if (!args || !args[0])
        return -1;

    if (strcmp(args[0], "exit")) {
        printf("Bye!\n");
        return 2;
    } else if (strcmp(args[0], "cd")) {
        changedir(args + 1);
        return -1;
    }

    return 0;
}

/*! \fn changedir
 * \brief Changes current directory of the host process.
 *
 */
int changedir(char **args) {
    if (!args)
        return -1;

    //static char *last_cwd = NULL;

    char *new_wd = args[0];

    if (!new_wd) {
        new_wd = getenv("HOME");
        printf("%s\n", new_wd);
    }

    if (*args[0] != '/') {
        if (strcmp(args[0], "~")) {
            new_wd = getenv("HOME");
        } else if (strcmp(args[0], "-")) {

        }
    } else {
    }

    chdir(new_wd);

    return 0;
}
