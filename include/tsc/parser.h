#pragma once

#include <tsc/sys.h>
#include <tsc/log.h>

typedef struct sTSParseData TSParseData;
typedef struct sTSFunctionData TSFunctionData;
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
  TS_IF = 0x7,
  TS_ELSE = 0x8,
  TS_RETURN = 0x9,
  TS_COMPONENT = 0x10,
  TS_UNKNOWN = 0x0,
} TSTokenType;

typedef enum eTSVisibility {
  TS_VISIBILITY_SCOPE = 0x0,
  TS_VISIBILITY_PRIVATE = 0x1,
  TS_VISIBILITY_PROTECTED = 0x2,
  TS_VISIBILITY_PUBLIC = 0x3
} TSVisibility;

typedef enum eTSConditionBodyTermination {
  TS_ENDS_WITH_BRACKET = 0x1,
  TS_ENDS_WITHOUT_BRACKET = 0x0,
} TSConditionBodyTermination;

typedef struct sTSKeyword {
  TSTokenType type;
  const char *str;
  TS_token_build_fn callback;
} TSKeyword;

#define KEYWORDS_SIZE 10

typedef struct sTSLocalVariableData {
  const char *name;
  const char *value;
} TSLocalVariableData;

typedef struct sTSFunctionData {
  const char *name;
  TSParserToken *arguments;
  u_long argumentsSize;
} TSFunctionData;

typedef struct sTSIfData {
  TSParserToken *conditions;
} TSIfData;

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
  TSVisibility visibility;
} TSParserToken;

typedef struct sTSFile {
  const char *file;
  TSParserToken *tokens;
  u_long tokensSize;
} TSFile;

const char *TS_getToken(FILE *stream);
const TSFile TS_parse_file(const char *file);
