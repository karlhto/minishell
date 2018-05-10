#ifndef __UTIL_H__MINISHELL
#define __UTIL_H__MINISHELL

#ifdef DEBUG
	#define DLOG(frmt, ...) \
		fprintf(stderr, frmt, __VA_ARGS__);
#else
	#define DLOG(frmt, args)
#endif

char **strsplt_ws(char *str);

#endif
