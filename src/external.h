#ifndef __EXTERNAL_H__MINISHELL
#define __EXTERNAL_H__MINISHELL

#include <sys/types.h>

pid_t fork_proc(char **args, int in, int out);

#endif
