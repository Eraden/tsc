#pragma once

#include <cts/sys.h>

typedef struct sTSParserToken TSParserToken;
typedef struct sTSFile TSFile;

typedef enum eTSOutputTargetType {
  TS_OUTPUT_UNSET,
  TS_OUTPUT_STREAM,
  TS_OUTPUT_STRING,
  TS_OUTPUT_FILE
} TSOutputTargetType;

typedef enum eTSFileSanity {
  TS_FILE_VALID,
  TS_FILE_SYNTAX_ERROR,
  TS_FILE_NOT_FOUND
}  __attribute__ ((__packed__)) TSFileSanity;

typedef struct sTSFile {
  struct {
    union {
      FILE *stream;
      char *outputFileName;
      wchar_t *buffer;
    };
    unsigned int indent;
    TSOutputTargetType type;
    TSParserToken *currentToken;
  } output;

  struct {
    wchar_t *file;
    FILE *stream;
  } input;

  struct {
    u_int line;
    u_int character;
    const wchar_t *token;
    FILE *stream;
    TSParserToken *parentTSToken;
  } parse;

  struct {
    unsigned char preserveComments;
    unsigned char highlight;
    unsigned int indentSize;
  } settings;

  TSParserToken **tokens;
  u_long tokensSize;

  TSFileSanity sanity;
  wchar_t *errorReason;
} TSFile;

TSFile *TS_build_file();

void TS_free_ts_file(TSFile *tsFile);
