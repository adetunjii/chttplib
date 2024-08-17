#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	
	char *str = "www.window.com";

	char *ret;

	ret = memchr(str, '.', strlen(str));
	size_t pos = 5;
	printf("%s", str + pos);

	return 0;
}
