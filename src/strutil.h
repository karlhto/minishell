/*! \file   strutil.h
 *
 */
#ifndef __STRUTIL_H__MINISHELL
#define __STRUTIL_H__MINISHELL

#ifdef DEBUG
#define DLOG(frmt, ...) \
	fprintf(stderr, frmt, __VA_ARGS__);
#else
#define DLOG(frmt, ...)
#endif

char **str_splt_ws(char *str);
char **str_splt(char *str, char *delims);

char ***str_parse(char *str, int *background);

#endif
