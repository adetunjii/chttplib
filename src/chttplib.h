#ifndef CHTTPLIB_H
#define CHTTPLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "ehandle.h"
#include "palloc.h"

/* Widely used macros and functions */

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

#endif /* CHTTPLIB_H */
