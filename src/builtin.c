#include "builtin.h"

#include <unistd.h>

int changedir(char **args) {
    static char *last_cwd = NULL;

    for (int i = 0; args[i]; i++) {
        if (i == 2) {

        }
    }

    return 0;
}
