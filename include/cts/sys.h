#pragma once

#include <cts/os.h>

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
#define TS_FILE_NOT_FOUND_CODE 5

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define swap(type, i, j) {type t = i; i = j; j = t;}

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

void ts_token_syntax_error(const wchar_t *msg, struct sTSFile *tsFile, const struct sTSParserToken *token, ...);

void ts_token_syntax_error_info(struct sTSFile *tsFile, const wchar_t *format, const wchar_t *msg);

void ts_log_position(const wchar_t *file, const u_long character, const u_long line);

const TSParserSettings TS_parse_arguments(int argc, const char **argv);

wchar_t *TS_join_strings(const wchar_t *, const wchar_t *);

void TS_suppress_logging(void (*fn)(void));

wchar_t *TS_resolve_directory(const wchar_t *absolute_path);

wchar_t *TS_resolve_file(const wchar_t *absolute_path);

wchar_t *TS_resolve_path(const wchar_t *absolute_path, const wchar_t *unresolved_path);
