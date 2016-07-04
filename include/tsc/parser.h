#pragma once

#include <tsc/sys.h>

typedef struct sTSParseData TSParseData;
typedef struct sTSKeyword TSKeyword;
typedef struct sTSParserToken TSParserToken;
typedef struct sTSFile TSFile;

typedef const TSParserToken (*TS_token_build_fn)(TSFile *tsFile, TSParseData *tsParseData);

typedef enum eTSTokenType {
  TS_VAR = 0x1,
  TS_LET = 0x2,
  TS_CONST = 0x3,
  TS_CLASS = 0x4,
  TS_FUNCTION = 0x5,
  TS_ARROW = 0x6,
  TS_UNKNOWN = 0x0,
} TSTokenType;

typedef struct sTSKeyword {
  TSTokenType type;
  const char *str;
  TS_token_build_fn callback;
} TSKeyword;

#define KEYWORDS_SIZE 6

typedef struct sTSLocalVariableData {
  const char *name;
  const char *value;
} TSLocalVariableData;

typedef struct sTSParseData {
  u_long position;
  u_long line;
  u_long character;
  const char *token;
  FILE *stream;
} TSParseData;

typedef struct sTSParserToken {
  u_long position;
  u_long line;
  u_long character;
  TSTokenType tokenType;
  u_long childrenSize;
  TSParserToken *children;
  void *data;
} TSParserToken;

typedef struct sTSFile {
  const char *file;
  TSParserToken *tokens;
  u_long tokensSize;
} TSFile;

const char *TS_getToken(FILE *stream);
const TSFile TS_parse_file(const char *file);
