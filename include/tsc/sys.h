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

const unsigned int TS_VERSION_MAJOR;
const unsigned int TS_VERSION_MINOR;
const unsigned int TS_VERSION_PATCH;
FILE *TS_stream_to_parse;
FILE *TS_output_stream;
const char *TS_file_name;

struct sTSFile;
struct sTSParserToken;

typedef enum eTSVerbosity {
  TS_VERBOSITY_OFF = 0x0,
  TS_VERBOSITY_ERROR = 0x1,
  TS_VERBOSITY_DEBUG = 0x2,
  TS_VERBOSITY_LOG = 0x3,
  TS_VERBOSITY_INFO = 0x4,
} __attribute__((__packed__)) TSVerbosity;

void TS_set_log_level(TSVerbosity verbosity);
unsigned char TS_check_log_level(TSVerbosity verbosity);

void ts_syntax_error(const char *msg, const char *file, const u_long character, const u_long line) __attribute__((noreturn));
void ts_token_syntax_error(const char *msg, const struct sTSFile *tsFile, const struct sTSParserToken *token) __attribute__((noreturn));
void ts_log_position(const char *file, const u_long character, const u_long line);

void TS_parse_arguments(int argc, const char **argv);
