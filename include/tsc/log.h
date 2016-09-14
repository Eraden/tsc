#pragma once

#ifdef Debug
#define TS_DEBUG 1
#endif

#include <tsc/sys.h>

void io_panic(wchar_t *msg);
void init_log(void);
void log_to_file(wchar_t *msg, ...);
void log_error(wchar_t *msg, ...);

void TS_set_error_output(FILE *output);
