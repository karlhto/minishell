/*! \file   strutil.c
 * Functions for parsing strings
 */
#include "strutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*! \fn strsplt
 * \brief A function for splitting strings.
 *        Splits string into several parts and returns an array.
 *
 * Utilises strtok to split a string. Returns a heap-allocated array appended
 * with a NULL pointer. Returned pointer needs to be freed.
 */
char **str_splt(char *str, char *delims) {
	if (!str || !delims) {
        fprintf(stderr,
                "strsplt(): input string or delimiter cannot be NULL\n");
		return NULL;
    }

	char **ret = NULL;
	int size = 1;

	char *tmp = strtok(str, delims);
	while (tmp != NULL) {
		ret = realloc(ret, sizeof(char *) * ++size);
		if (!ret) {
			fprintf(stderr, "strsplt(): memory allocation failed\n");
			return NULL;
		}

		ret[size - 2] = tmp;
		ret[size - 1] = NULL;
		tmp = strtok(NULL, delims);
	}

	return ret;
}

/*! \fn strsplt_ws
 *  \brief Wrapper function for splitting on all whitespace in a string.
 *
 * Returns a heap-allocated array of strings appended by a NULL pointer.
 * Returned pointer needs to be freed.
 */
char **str_splt_ws(char *str) {
    return str_splt(str, " \t\n");
}

/*! \fn parse_str
 * \brief Parses string into several pipes.
 *
 * First runs through the string to look for ampersands, in which case found
 * process is to be run in the background. If pipes are found, it splits the
 * programs into several elements. The element(s) is/are then split by
 * whitespace. Returns the full structure.
 *
 * It returns
 */
char ***str_parse(char *str, int *background) {
    if (!str)
        return NULL;

    // this pointer persists through each run-through of str_parse
    static char *strptr = NULL;

    if (!strptr) {
        str = strtok(str, "&");
        strptr = strtok(NULL, "&");
    } else {
        str = strtok(strptr, "&");
        strptr = strtok(NULL, "&");
    }

    // if strptr is not NULL, background is set.
    if (strptr)
        *background = 1;
    else
        *background = 0;

    char ***ret = malloc(sizeof(char *));
    ret[0] = NULL;

    char **procs = str_splt(str, "|");
    for (int i = 0; procs[i]; i++) {
        ret = realloc(ret, sizeof(char *) * (i + 2));
        if (!ret) {
            fprintf(stderr, "parse_str(): memory allocation failed");
            return NULL;
        }

        ret[i] = str_splt_ws(procs[i]);
        ret[i + 1] = NULL;
    }

    free(procs);

    return ret;
}
