#ifndef CHTTPLIB_H
#define CHTTPLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ehandle.h"
#include "palloc.h"

/* Widely used macros */

/* 
 *  Max
 *      Returns the maximum of two numbers.
 */
#define Max(a, b)   ((a) > (b) ? (a) : (b))

/*
 * Min
 *      Returns the minimum of two numbers.
 */
#define Min(a, b)   ((a) < (b) ? (a) : (b))

static inline void toLowercase(char *str) {
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++) str[i] = tolower(str[i]);
}

#endif /* CHTTPLIB_H */
