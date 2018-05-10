#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * A function for splitting strings.
 * Utilises strtok to split a string into several parts.
 */
char **strsplt_ws(char *str) {
	if (!str)
		return NULL;

	char **ret = NULL;
	char delims[4] = {' ', '\t', '\n', 0};
	int size = 1;

	char *tmp = strtok(str, delims);
	while (tmp) {
		ret = realloc(ret, sizeof(char *) * (size + 1));
		if (!ret) {
			fprintf(stderr, "realloc(): memory allocation failed\n");
			return NULL;
		}

		ret[size - 1] = tmp;
		ret[size++] = NULL;
		tmp = strtok(NULL, delims);
	}

	return ret;
}
