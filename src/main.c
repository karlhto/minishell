/**
 * Minishell.
 * Small program for emulating shell.
 */
#include "shell.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Main.
 *
 *
 * @param	int		argc
 * @param	char	**argv
 *
 * @return	EXIT_SUCCESS
 */
int main(void) {
    DLOG("test(): %s\n", "failed doing stuff");

    if (run_shell()) {
        perror("run_shell()");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
