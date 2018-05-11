/*! \file   builtin.c
 * Handles all builtin commands.
 */
#include "builtin.h"
#include "strutil.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char *name;
    int (*func) (char **);
} builtin_t;

static int num_funcs();
static int msh_cd(char **args);
static int msh_exit(char **args);

static int set_cwd(char *ptr);

static char *cur_wd = NULL;

static builtin_t builtin_cmds[] = {
    {"cd", &msh_cd},
    {"exit", &msh_exit}
};

/*! \fn init_builtin
 * \brief Initialises resources for builtin.
 *
 * Allocates memory for current working directory and sets it.
 */
int init_builtin() {
    uint32_t path_max = pathconf(".", _PC_PATH_MAX);

    cur_wd = calloc(path_max, sizeof(char));
    if (!cur_wd) {
        fprintf(stderr, "init_builtin(): memory allocation failed");
        return BUILTIN_ERROR;
    }

    if (set_cwd(cur_wd) == BUILTIN_ERROR)
        return BUILTIN_ERROR;

    return BUILTIN_SUCCESS;
}

/*! \fn free_builtin
 * Frees current CWD buffer.
 */
void free_builtin() {
    if (cur_wd)
        free(cur_wd);
}

/*! \fn exec_builtin
 * Executes a builtin command in the library.
 */
int exec_builtin(char **args) {
    if (!args || !args[0])
        return BUILTIN_SUCCESS;

    int n = num_funcs();
    for (int i = 0; i < n; i++) {
        if (strcmp(args[0], builtin_cmds[i].name) == 0) {
            return (*builtin_cmds[i].func)(args + 1);
        }
    }

    return BUILTIN_NOT_FOUND;
}

/*! \fn pwd_builtin
 * Prints the current directory to mark location.
 */
void pwd_builtin() {
    printf("%s>", cur_wd);
}

/*! \fn num_funcs
 * Returns number of functions in builtin_cmds.
 */
static int num_funcs() {
    return sizeof(builtin_cmds) / sizeof(builtin_t);
}

/*! \fn msh_chdir
 * \brief Changes current directory of the host process.
 *
 * Changes current directory. If args is NULL, returns an error. If no
 * argument is supplied (if args[0] is NULL) or first argument is ~ or --,
 * redirect to home. Sets static global variable cur_wd after chdir is
 * successful.
 */
static int msh_cd(char **args) {
    if (!args) {
        fprintf(stderr, "msh_cd(): args cannot be NULL\n");
        return BUILTIN_ERROR;
    }

    char *new_wd = args[0];

    if (!new_wd) {
        new_wd = getenv("HOME");
    } else if (new_wd[0] != '/') {
        if (strcmp(new_wd, "~") == 0 || strcmp(new_wd, "--") == 0)
            new_wd = getenv("HOME");
    }

    DLOG("Directory to be changed to: %s\n", new_wd);

    if (chdir(new_wd)) {
        perror("cd");
        return BUILTIN_SUCCESS;
    }

    if (set_cwd(cur_wd) == BUILTIN_ERROR)
        return BUILTIN_ERROR;

    return BUILTIN_SUCCESS;
}

/*! \fn msh_exit
 * Prints input argument or standard text and return 2.
 */
static int msh_exit(char **args) {
    if (args[0])
        printf("%s\n", args[0]);
    else
        printf("Bye! :)\n");

    return BUILTIN_EXIT;
}

/*! \fn newdir
 *
 * Changes ptr to current working directory.
 */
static int set_cwd(char *ptr) {
    if (!ptr)
        return BUILTIN_ERROR;

    uint32_t path_max = pathconf(".", _PC_PATH_MAX);
    char *status = getcwd(ptr, path_max);
    if (!status)
        return BUILTIN_ERROR;

    return BUILTIN_SUCCESS;
}
