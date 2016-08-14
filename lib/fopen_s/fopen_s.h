#include <stdio.h>
#include <errno.h>

#ifndef COMPAT_FOPEN_S_H
#define COMPAT_FOPEN_S_H

#ifndef _WIN32
	typedef int errno_t;
#endif

errno_t fopen_s(FILE **f, const char *name, const char *mode);

#endif /* COMPAT_FOPEN_S_H */
