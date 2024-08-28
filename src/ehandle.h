/*
 *
 * ehandle.h
 *	  chttplib error reporting/logging definitions.
 */

#ifndef EHANDLE_H
#define EHANDLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#define DEBUG		1
#define INFO		2
#define WARNING		3
#define ERROR		4

extern char *errmsg(const char *fmt, ...);
extern void write_stderr(const char *errmsg, size_t len);

// TODO: improve error handling.
#define ehandle(elevel, err_msg) \
	do { \
		write_stderr(err_msg, strlen(err_msg)); \
		if(elevel >= ERROR) abort(); \
	} while(0)

#endif /* EHANDLE_H */
