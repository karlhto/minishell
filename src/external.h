#ifndef __EXTERNAL_H__MINISHELL
#define __EXTERNAL_H__MINISHELL

#include <sys/types.h>

int pipe_procs(char ***procs, int background);
pid_t fork_proc(char **args, int in, int out);

#endif
