#pragma once

#include <tsc/sys.h>
#include <tsc/log.h>

#define TS_TOKEN_BEGIN(token) log_to_file(L"-> parsing as %s\n", token);
#define TS_TOKEN_END(token) log_to_file(L"-> end %s\n", token);
#define TS_STRING_END 1

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
  TS_DECORATOR = 0x10,
  TS_IMPORT = 0x11,
  TS_EXPORT = 0x12,
  TS_DEFAULT = 0x13,
  TS_SCOPE = 0x14,
  TS_EXTENDS = 0x15,
  TS_IMPLEMENTS = 0x16,
  TS_NEW = 0x17,
  TS_CLASS_FIELD = 0x18,
  TS_CLASS_METHOD = 0x19,
  TS_INLINE_COMMENT = 0x20,
  TS_MULTILINE_COMMENT = 0x21,
  TS_CONDITION = 0x22,
  TS_ARGUMENT = 0x23,
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
  TS_PARSE_VARIABLE_NONE = 0x0,
  TS_PARSE_VARIABLE_NAME = 0x1,
  TS_PARSE_VARIABLE_TYPE = 0x2,
  TS_PARSE_VARIABLE_VALUE = 0x3,
} __attribute__((__packed__)) TSVariableParseFlag;

typedef enum eTSVisibility {
  TS_VISIBILITY_NONE = 0x0,
  TS_VISIBILITY_SCOPE = 0x1,
  TS_VISIBILITY_PRIVATE = 0x2,
  TS_VISIBILITY_PROTECTED = 0x3,
  TS_VISIBILITY_PUBLIC = 0x4
} __attribute__ ((__packed__)) TSVisibility;

typedef enum eTSConditionBodyTermination {
  TS_ENDS_WITH_BRACKET = 0x1,
  TS_ENDS_WITHOUT_BRACKET = 0x0,
} __attribute__ ((__packed__)) TSConditionBodyTermination;

typedef struct sTSKeyword {
  TSTokenType __attribute__((__unused__)) type;
  const wchar_t *str;
  TS_token_build_fn callback;
} TSKeyword;

#define KEYWORDS_SIZE 19

typedef struct sTSLocalVariableData {
  const wchar_t *name;
  const wchar_t *value;
  const wchar_t *type;
} TSLocalVariableData;

typedef struct sTSFunctionData {
  const wchar_t *name;
  TSParserToken *arguments;
  u_long argumentsSize;
  const wchar_t *returnType;
} TSFunctionData;

typedef struct sTSIfData {
  TSParserToken *conditions;
  u_long conditionsSize;
} TSIfData;

typedef struct sTSClassData {
  const wchar_t *name;
  const wchar_t *parentClass;
  TSParserToken *__attribute__((__unused__)) implementsInterfaces;
  u_long __attribute__((__unused__)) implementsInterfacesSize;
} TSClassData;

typedef struct sTSParseData {
  u_long position;
  u_long line;
  u_long character;
  const wchar_t *token;
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
  FILE *stream;
  TSParserToken *tokens;
  u_long tokensSize;
} TSFile;

void TS_put_back(FILE *stream, const wchar_t *value);

unsigned char TS_name_is_valid(const wchar_t *name);

void TS_push_child(TSParserToken *token, TSParserToken child);

void TS_free_unknown(TSParserToken token);

const TSParserToken TS_parse_var(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_var(TSParserToken token);

const TSParserToken TS_parse_let(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_let(TSParserToken token);

const TSParserToken TS_parse_const(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_const(TSParserToken token);

const TSParserToken TS_parse_class(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_class(const TSParserToken token);
void TS_free_class_field(const TSParserToken token);
void TS_free_class_method(const TSParserToken token);

const TSParserToken TS_parse_function(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_function(const TSParserToken token);

const TSParserToken TS_parse_arrow(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_arrow(const TSParserToken token);

const TSParserToken TS_parse_if(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_if(const TSParserToken token);

const TSParserToken TS_parse_else(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_else(const TSParserToken token);

const TSParserToken TS_parse_decorator(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_decorator(const TSParserToken token);

const TSParserToken TS_parse_return(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_return(const TSParserToken token);

const TSParserToken TS_parse_import(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_import(const TSParserToken token);

const TSParserToken TS_parse_export(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_export(const TSParserToken token);

const TSParserToken TS_parse_default(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_default(const TSParserToken token);

const TSParserToken TS_parse_scope(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_scope(const TSParserToken token);

const TSParserToken TS_parse_extends(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_extends(const TSParserToken token);

const TSParserToken TS_parse_implements(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_implements(const TSParserToken token);

const TSParserToken TS_parse_new(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_new(const TSParserToken token);

const TSParserToken TS_parse_inline_comment(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_inline_comment(const TSParserToken token);

const TSParserToken TS_parse_multiline_comment(TSFile *tsFile, TSParseData *tsParseData);
void TS_free_multiline_comment(const TSParserToken token);

TSParserToken TS_parse_ts_token(TSFile *tsFile, TSParseData *data);
void TS_free_tsToken(const TSParserToken token);
void TS_free_children(const TSParserToken token);

volatile const wchar_t *TS_getToken(FILE *stream) __attribute__((__malloc__));

const wchar_t *TS_clone_string(const wchar_t *string) __attribute__((__malloc__));

const TSFile TS_parse_file(const char *fileName);

const TSFile TS_parse_stream(const char *file, FILE *stream);

void TS_free_tsFile(const TSFile tsFile);
