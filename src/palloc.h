#ifndef PALLOC_H
#define PALLOC_H

#include <stdlib.h>

#define MaxAllocSize	((size_t) 0x3fffffff) /* 1 gigabyte - 1 */

#define AllocSizeIsValid(size)	((size_t) (size) <= MaxAllocSize)

extern void *palloc(size_t sz);
extern void pfree(void *ptr);

#endif /* PALLOC_H */
