#pragma once

#include <cts/sys.h>
#include <cts/log.h>

#define TS_TOKEN_BEGIN(token) log_to_file(L"-> parsing as %s\n", token);
#define TS_TOKEN_SECTION_BEGIN(token) log_to_file(L"    -> parsing as %s\n", token);
#define TS_TOKEN_END(token) log_to_file(L"-> end %s\n", token);
#define TS_TOKEN_SECTION_END(token) log_to_file(L"    -> end %s\n", token);
#define TS_STRING_END 1
#define TS_NEW_TOKEN calloc(sizeof(TSParserToken), 1)
#define TS_LOOP_SANITY_CHECK(file) if (file->sanity != TS_FILE_VALID) break;
#define TS_MOVE_BY(data, tok) { u_long m = wcslen(tok); data->position += m; data->position += m; }
#define TS_NEW_LINE(data, tok) { u_long m = wcslen(tok); data->character = 0; data->position += m; data->line += 1; }
#define TS_UNEXPECTED_END_OF_STREAM(file, token, type) ts_token_syntax_error( \
  (const wchar_t *) L"Unexpected end of stream while parsing " #type, \
  file, token )
#define TS_UNEXPECTED_TOKEN(tsFile, token, tok, type) ts_token_syntax_error( \
  (const wchar_t *) L"Unexpected token while parsing " #type, \
  tsFile, token, tok );

typedef struct sTSParseData TSParseData;
typedef struct sTSFunctionData TSFunctionData;
typedef struct sTSKeyword TSKeyword;
typedef struct sTSParserToken TSParserToken;
typedef struct sTSFile TSFile;

typedef TSParserToken *(*TS_token_build_fn)(TSFile *tsFile, TSParseData *tsParseData);

typedef enum eTSFileSanity {
  TS_FILE_VALID,
  TS_FILE_SYNTAX_ERROR,
  TS_FILE_NOT_FOUND
}  __attribute__ ((__packed__)) TSFileSanity;

typedef enum eTSTokenType {
  TS_VAR = 1,
  TS_LET = 2,
  TS_CONST = 3,
  TS_CLASS = 4,
  TS_FUNCTION = 5,
  TS_ARROW = 6,
  TS_IF = 7,
  TS_ELSE = 8,
  TS_RETURN = 9,
  TS_DECORATOR = 10,
  TS_IMPORT = 11,
  TS_EXPORT = 12,
  TS_DEFAULT = 13,
  TS_SCOPE = 14,
  TS_EXTENDS = 15,
  TS_IMPLEMENTS = 16,
  TS_NEW = 17,
  TS_CLASS_FIELD = 18,
  TS_CLASS_METHOD = 19,
  TS_INLINE_COMMENT = 20,
  TS_MULTILINE_COMMENT = 21,
  TS_CONDITION = 22,
  TS_ARGUMENT = 23,
  TS_CALLER = 24,
  TS_SWITCH = 25,
  TS_CASE = 26,
  TS_BREAK = 27,
  TS_FOR = 28,
  TS_FOR_LET = 29,
  TS_FOR_IN = 30,
  TS_FOR_OF = 31,
  TS_UNKNOWN = 0,
} __attribute__ ((__packed__)) TSTokenType;

typedef enum sTSParseArgumentFlag {
  TS_PARSE_ARG_NAME = 0x0,
  TS_PARSE_ARG_TYPE = 0x1,
  TS_PARSE_ARG_VALUE = 0x2,
} __attribute__ ((__packed__)) TSParseArgumentFlag;

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
  TSTokenType type;
  const wchar_t *str;
  TS_token_build_fn callback;
} TSKeyword;

#define KEYWORDS_SIZE 23

typedef struct sTSLocalVariableData {
  const wchar_t *name;
  const wchar_t *value;
  const wchar_t *type;
} TSLocalVariableData;

typedef struct sTSFunctionData {
  const wchar_t *name;
  const wchar_t *returnType;
} TSFunctionData;

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
  TSParserToken *parentTSToken;
} TSParseData;

typedef struct sTSParserToken {
  TSTokenType tokenType;
  u_long line;
  u_long character;
  u_long position;
  u_long childrenSize;
  TSParserToken **children;
  TSVisibility visibility;
  TSParserToken *parent;
  union {
    void *data;
    wchar_t *name;
    wchar_t *content;
    TSClassData *classData;
    TSFunctionData *functionData;
    TSLocalVariableData *variableData;
  };
} TSParserToken;

typedef struct sTSFile {
  wchar_t *file;
  wchar_t *errorReason;
  FILE *stream;
  TSParserToken **tokens;
  u_long tokensSize;
  TSFileSanity sanity;
} TSFile;

unsigned char TS_is_keyword(const wchar_t *str);

TSParserToken *TS_build_parser_token(TSTokenType tokenType, TSParseData *tsParseData);

void TS_put_back(FILE *stream, volatile const wchar_t *value);

unsigned char TS_name_is_valid(const wchar_t *name);

void TS_push_child(TSParserToken *token, TSParserToken *child);

void TS_free_unknown(const TSParserToken *token);

TSParserToken *TS_parse_condition(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_condition(const TSParserToken *token);

TSParserToken *TS_parse_switch(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_switch(const TSParserToken *token);

TSParserToken *TS_parse_case(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_case(const TSParserToken *token);

TSParserToken *TS_parse_break(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_break(const TSParserToken *token);

TSParserToken *TS_parse_argument(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_argument(const TSParserToken *token);

TSParserToken *TS_parse_var(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_var(const TSParserToken *token);

TSParserToken *TS_parse_let(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_let(const TSParserToken *token);

TSParserToken *TS_parse_const(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_const(const TSParserToken *token);

TSParserToken *TS_parse_class(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_class(const TSParserToken *token);

void TS_free_class_field(const TSParserToken *token);

void TS_free_class_method(const TSParserToken *token);

TSParserToken *TS_parse_function(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_function(const TSParserToken *token);

TSParserToken *TS_parse_arrow(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_arrow(const TSParserToken *token);

TSParserToken *TS_parse_if(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_if(const TSParserToken *token);

TSParserToken *TS_parse_else(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_else(const TSParserToken *token);

TSParserToken *TS_parse_decorator(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_decorator(const TSParserToken *token);

TSParserToken *TS_parse_return(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_return(const TSParserToken *token);

TSParserToken *TS_parse_import(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_import(const TSParserToken *token);

TSParserToken *TS_parse_export(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_export(const TSParserToken *token);

TSParserToken *TS_parse_default(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_default(const TSParserToken *token);

TSParserToken *TS_parse_scope(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_scope(const TSParserToken *token);

TSParserToken *TS_parse_extends(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_extends(const TSParserToken *token);

TSParserToken *TS_parse_implements(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_implements(const TSParserToken *token);

TSParserToken *TS_parse_new(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_new(const TSParserToken *token);

TSParserToken *TS_parse_inline_comment(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_inline_comment(const TSParserToken *token);

TSParserToken *TS_parse_multiline_comment(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_multiline_comment(const TSParserToken *token);

TSParserToken *TS_parse_caller(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_caller(const TSParserToken *token);

TSParserToken *TS_parse_for(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_for(const TSParserToken *token);

TSParserToken *TS_parse_ts_token(TSFile *tsFile, TSParseData *data);

void TS_free_tsToken(const TSParserToken *token);

void TS_free_children(const TSParserToken *token);

volatile const wchar_t *TS_getToken(FILE *stream) __attribute__((__malloc__));

wchar_t *TS_clone_string(const wchar_t *string) __attribute__((__malloc__));

TSFile *TS_parse_file(const char *fileName);

TSFile *TS_parse_stream(const char *file, FILE *stream);

void TS_free_tsFile(TSFile *tsFile);
