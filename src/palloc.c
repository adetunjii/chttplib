#include "palloc.h"
#include "ehandle.h"

void *palloc(size_t sz) {
	void *mem;
	
	if (!AllocSizeIsValid(sz)) {
		ehandle(ERROR, errmsg("invalid memory size requested %zu", sz));
	}
	
	mem = malloc(sz);
	if (mem == NULL) {
		ehandle(ERROR, errmsg("out of memory"));
	}
	return mem;
}

void pfree(void *ptr) {
	free(ptr);
}
