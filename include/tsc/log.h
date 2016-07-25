#pragma once

#ifdef Debug
#define TS_DEBUG 1
#endif

#include <tsc/sys.h>

void io_panic(const char *msg);
void init_log();
void log_to_file(char *msg, ...);
void log_error(char *msg, ...);
