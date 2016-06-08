#pragma once

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

#if defined __APPLE__
#include <unistd.h>
#include <sys/stat.h>
#else
#include <sys/unistd.h>
#include <sys/stat.h>
#endif

