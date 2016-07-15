#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <assert.h>
#include <errno.h>

#if defined __APPLE__
#include <unistd.h>
#include <sys/stat.h>
#else
#include <sys/unistd.h>
#include <sys/stat.h>
#endif

#define SYNTAX_ERROR { exit(2); }

void ts_syntax_error(const char *msg, const char *file, const u_long character, const u_long line) __attribute__((noreturn));
void ts_log_position(const char *file, const u_long character, const u_long line);
