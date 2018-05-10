/*! \file   util.c
 *
 */
#include "util.h"

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
char **strsplt(char *str, char *delims) {
	if (!str || !delims) {
        fprintf(stderr,
                "strsplt(): input string or delimiter cannot be NULL\n");
		return NULL;
    }

	char **ret = NULL;
	int size = 1;

	char *tmp = strtok(str, delims);
	while (tmp) {
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
char **strsplt_ws(char *str) {
    return strsplt(str, " \r\f\v\t\n");
}
