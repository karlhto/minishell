/*! \file   main.c
 * Entry point of minishell.
 */
#include "shell.h"
#include "strutil.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int arghandler(int argc, char **argv);
static void display_usage(char const *filename);

/*! \fn main
 * \brief Entry point of program.
 * Handles arguments and enters shell REPL.
 */
int main(int argc, char **argv) {
    int ret = arghandler(argc, argv);
    if (ret == 1) {
        exit(EXIT_SUCCESS);
    } else if (ret == -1) {
        exit(EXIT_FAILURE);
    }

    if (run_shell()) {
        perror("run_shell()");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

/*! \fn arghandler
 * \brief Handles args.
 *
 * Currently pretty empty, new arguments will be added eventually.
 */
static int arghandler(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
            case 'h':
                display_usage(argv[0]);
                return 1;
            default:
                return -1;
        }
    }

    return 0;
}

/*! \fn display_usage
 * Prints usage of program.
 */
static void display_usage(char const *filename) {
    printf("Usage: %s\n", filename);
}
