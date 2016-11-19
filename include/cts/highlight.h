#pragma once

#include <cts/sys.h>

#define TS_COLOR_NORMAL  "\x1B[0m"
#define TS_COLOR_RED  "\x1B[31m"
#define TS_COLOR_GREEN  "\x1B[32m"
#define TS_COLOR_YELLOW  "\x1B[33m"
#define TS_COLOR_BLUE  "\x1B[34m"
#define TS_COLOR_MAGENDA  "\x1B[35m"
#define TS_COLOR_CYNAN  "\x1B[36m"
#define TS_COLOR_WHITE  "\x1B[37m"

void TS_highlight(const char *color, FILE *file);

void TS_highlight_code(const char *color);

void TS_highlight_error();

void TS_clear_highlight();
