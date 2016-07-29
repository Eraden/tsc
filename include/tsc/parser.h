#pragma once

#include <tsc/sys.h>
#include <tsc/log.h>

#define TS_TOKEN_BEGIN(token) log_to_file("-> parsing as %s\n", token);
#define TS_TOKEN_END(token) log_to_file("-> end %s\n", token);
#define TS_STRING_END sizeof(char)

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
  TS_EXTENDS = 0x15,
  TS_IMPLEMENTS = 0x16,
  TS_NEW = 0x17,
  TS_CLASS_FIELD = 0x18,
  TS_CLASS_METHOD = 0x19,
  TS_UNKNOWN = 0x0,
} __attribute__ ((__packed__)) TSTokenType;

typedef enum sTSFunctionParseFlag {
  TS_PARSE_FN_ARG_NAME = 0x0,
  TS_PARSE_FN_ARG_TYPE = 0x1,
  TS_PARSE_FN_ARG_VALUE = 0x2,
} TSFunctionParseFlag;

typedef enum sTSClassParseFlag {
  TS_PARSE_CLASS_MEMBER_NAME = 0x0,
  TS_PARSE_CLASS_MEMBER_TYPE = 0x1,
  TS_PARSE_CLASS_MEMBER_VALUE = 0x2,
  TS_PARSE_CLASS_MEMBER_METHOD_ARGUMENTS = 0x3,
  TS_PARSE_CLASS_MEMBER_METHOD_BODY = 0x4,
  TS_PARSE_CLASS_MEMBER_METHOD_RETURN_TYPE = 0x5,
} __attribute__((__packed__)) TSClassParseFlag;

typedef enum sTSVariableParseFlag {
  TS_PARSE_VARIABLE_NAME = 0x0,
  TS_PARSE_VARIABLE_TYPE = 0x1,
  TS_PARSE_VARIABLE_VALUE = 0x2,
} __attribute__((__packed__)) TSVariableParseFlag;

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
  TSTokenType __attribute__((__unused__)) type;
  const char *str;
  TS_token_build_fn callback;
} TSKeyword;

#define KEYWORDS_SIZE 17

typedef struct sTSLocalVariableData {
  const char *name;
  const char *value;
  const char *type;
} TSLocalVariableData;

typedef struct sTSFunctionData {
  const char *name;
  TSParserToken *arguments;
  u_long argumentsSize;
  const char *returnType;
} TSFunctionData;

typedef struct sTSIfData {
  TSParserToken *conditions;
  u_long conditionsSize;
} TSIfData;

typedef struct sTSClassData {
  const char *name;
  const char *parentClass;
  TSParserToken *__attribute__((__unused__)) implementsInterfaces;
  u_long __attribute__((__unused__)) implementsInterfacesSize;
} TSClassData;

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

void TS_put_back(FILE *stream, const char *value);

unsigned char TS_name_is_valid(const char *name);

void TS_push_child(TSParserToken *token, TSParserToken child);

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

const TSParserToken TS_parse_extends(TSFile *tsFile, TSParseData *tsParseData);

const TSParserToken TS_parse_implements(TSFile *tsFile, TSParseData *tsParseData);

const TSParserToken TS_parse_new(TSFile *tsFile, TSParseData *tsParseData);

TSParserToken TS_parse_ts_token(TSFile *tsFile, TSParseData *data);

volatile const char *TS_getToken(FILE *stream) __attribute__((__malloc__));

const char *TS_clone_string(const char *string) __attribute__((__malloc__));

const TSFile TS_parse_file();

const TSFile TS_parse_stream(const char *file, FILE *stream);
