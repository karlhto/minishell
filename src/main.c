/*! \file   main.c
 * Entry point of minishell.
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
    if (run_shell()) {
        perror("run_shell()");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

static void display_usage(char *filename) {
    printf("Usage: %s\n", filename);
}
