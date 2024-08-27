#include "ehandle.h"

char *errmsg(const char *fmt,...) {
	char *buf;

	va_list args;
	va_start(args, fmt);

	int len = vsnprintf(NULL, 0, fmt, args);
	if (len <= 0) {
		va_end(args);
		return NULL;
	}

	buf = malloc(sizeof(char) * len);
	if (buf == NULL) {
		va_end(args);
		return NULL;
	}

	vsnprintf(buf, len, fmt, args);
	va_end(args);

	return buf;
}

void write_stderr(const char *line,  size_t len) {
	int n;
	n = write(fileno(stderr), line, len);
	(void) n;
}
