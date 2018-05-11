#ifndef __BUILTIN_H__MINISHELL
#define __BUILTIN_H__MINISHELL

enum {
    BUILTIN_ERROR = -1,
    BUILTIN_NOT_FOUND,
    BUILTIN_EXIT,
    BUILTIN_SUCCESS,
};

int init_builtin();
void free_builtin();
void pwd_builtin();

int exec_builtin(char **args);

#endif
