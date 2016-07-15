#pragma once

#ifdef Debug
#endif
#define TS_DEBUG 1

#include <tsc/sys.h>

void io_panic(const char *msg);
void init_log();
void log_to_file(char *msg, ...);
void log_error(char *msg, ...);
