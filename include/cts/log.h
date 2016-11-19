#pragma once

#ifdef Debug
#define TS_DEBUG 1
#endif

#include <cts/sys.h>
#include <cts/highlight.h>

void TS_io_panic(wchar_t *msg);
void TS_init_log(void);
void TS_log_to_file(wchar_t *msg, ...);
void log_error(wchar_t *msg, ...);

void TS_set_error_output(FILE *output);
