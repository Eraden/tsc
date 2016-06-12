#pragma once

#include <tsc/sys.h>
#include <tsc/macro.h>

// String
char *appendChar(const char *content, char c);
char *cloneString(const char *content);
char *concat(char *a, char *b);

#define CONCAT(dest, src) dest = concat(dest, src);
