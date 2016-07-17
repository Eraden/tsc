#pragma once

#include <tsc/sys.h>
#include <tsc/log.h>

#define TS_TOKEN_BEGIN(token) log_to_file("-> parsing as %s\n", token);
#define TS_TOKEN_END(token) log_to_file("-> end %s\n", token);

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
  TS_IMPORT = 0x11,
  TS_EXPORT = 0x12,
  TS_DEFAULT = 0x13,
  TS_SCOPE = 0x14,
  TS_UNKNOWN = 0x0,
} __attribute__ ((__packed__)) TSTokenType;

typedef enum eTSVisibility {
  TS_VISIBILITY_SCOPE = 0x0,
  TS_VISIBILITY_PRIVATE = 0x1,
  TS_VISIBILITY_PROTECTED = 0x2,
  TS_VISIBILITY_PUBLIC = 0x3
} __attribute__ ((__packed__)) TSVisibility;

typedef enum eTSConditionBodyTermination {
  TS_ENDS_WITH_BRACKET = 0x1,
  TS_ENDS_WITHOUT_BRACKET = 0x0,
} __attribute__ ((__packed__)) TSConditionBodyTermination;

typedef struct sTSKeyword {
  TSTokenType type;
  const char *str;
  TS_token_build_fn callback;
} TSKeyword;

#define KEYWORDS_SIZE 14

typedef struct sTSLocalVariableData {
  const char *name;
  const char *value;
  const char *type;
} TSLocalVariableData;

typedef struct sTSFunctionData {
  const char *name;
  TSParserToken *arguments;
  u_long argumentsSize;
} TSFunctionData;

typedef struct sTSIfData {
  TSParserToken *conditions;
  u_long conditionsSize;
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
} TSFile ;

void TS_put_back(FILE *stream, const char *value);

unsigned char TS_name_is_valid(const char *name);

const TSParserToken TS_parse_var(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_let(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_const(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_class(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_function(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_arrow(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_if(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_else(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_decorator(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_return(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_import(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_export(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_default(TSFile *tsFile, TSParseData *tsParseData);
const TSParserToken TS_parse_scope(TSFile *tsFile, TSParseData *tsParseData);

TSParserToken TS_parse_ts_token(TSFile *tsFile, TSParseData *data);

volatile const char *TS_getToken(FILE *stream) __attribute__((__malloc__));
const TSFile TS_parse_file(const char *file);
const char *TS_clone_string(const char *string) __attribute__((__malloc__));
