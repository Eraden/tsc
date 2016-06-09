#pragma once

#include <tsc/sys.h>
#include <tsc/macro.h>

// String
char *appendChar(const char *content, char c);
char *cloneString(const char *content);
char *concat(const char *a, const char *b);

#define CONCAT(dest, src) { char *b = concat(dest, src); free(dest); dest = b; }
