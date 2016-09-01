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
#include <wchar.h>
#include <locale.h>
#include <pthread.h>

#ifdef __APPLE__
#include <unistd.h>
#include <sys/stat.h>
#else
#include <sys/unistd.h>
#include <sys/stat.h>
#endif

#define TS_PARSE_FAILURE_CODE 4

const unsigned int TS_VERSION_MAJOR;
const unsigned int TS_VERSION_MINOR;
const unsigned int TS_VERSION_PATCH;
FILE *TS_output_stream;

struct sTSFile;
struct sTSParserToken;

typedef enum eTSVerbosity {
  TS_VERBOSITY_OFF = 0x0,
  TS_VERBOSITY_ERROR = 0x1,
  TS_VERBOSITY_DEBUG = 0x2,
  TS_VERBOSITY_LOG = 0x3,
  TS_VERBOSITY_INFO = 0x4,
} __attribute__((__packed__)) TSVerbosity;

typedef struct sTSParserSettings {
  const char *fileName;
  FILE *stream;
} TSParserSettings;

void TS_set_log_level(TSVerbosity verbosity);
unsigned char TS_check_log_level(TSVerbosity verbosity);

void ts_token_syntax_error(const wchar_t *msg, const struct sTSFile *tsFile, const struct sTSParserToken *token) __attribute__((noreturn));
void ts_log_position(const wchar_t *file, const u_long character, const u_long line);

const TSParserSettings TS_parse_arguments(int argc, const char **argv);
