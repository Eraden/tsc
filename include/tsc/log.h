#pragma once

#include <tsc/sys.h>

void init_log();
void log_to_file(char *msg, ...);
void log_error(char *msg, ...);

