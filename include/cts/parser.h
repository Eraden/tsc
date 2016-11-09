#pragma once

#include <cts/sys.h>
#include <cts/log.h>

#define TS_CODE_EVAL "(code eval)"
#define TS_TOKEN_SECTION_BEGIN(token) TS_log_to_file(L"    -> parsing as %s\n", token);
#define TS_TOKEN_SECTION_END(token) TS_log_to_file(L"    -> end %s\n", token);
#define TS_STRING_END 1
#define TS_NEW_TOKEN calloc(sizeof(TSParserToken), 1)
#define TS_LOOP_SANITY_CHECK(file) if (file->sanity != TS_FILE_VALID) break;
#define TS_MOVE_BY(data, tok) { u_long m = wcslen(tok); data->character += m; }
#define TS_NEW_LINE(data, tok) { u_long m = wcslen(tok); data->character = 0; data->line += 1; }

#define TS_UNEXPECTED_END_OF_STREAM(file, token, type) ts_token_syntax_error( \
  (const wchar_t *) L"Unexpected end of stream while parsing `" type "`", \
  file, token )
#define TS_UNEXPECTED_TOKEN(tsFile, token, tok, type) ts_token_syntax_error( \
  (const wchar_t *) L"Unexpected token while parsing `" type "`", \
  tsFile, token, tok ); \
  ts_token_syntax_error_info(tsFile, (const wchar_t *) L"invalid token: \"%ls\"", (wchar_t *) tok);
#define TS_UNKNOWN_TYPE(tsFile, token, name) ts_token_syntax_error( \
  (const wchar_t *) L"Unknown type", tsFile, token ); \
  ts_token_syntax_error_info(tsFile, (const wchar_t *) L"%ls", (wchar_t *) name);
/*#define TS_UNKNOWN_TYPE(tsFile, token, name) \
  ts_token_syntax_error_info(tsFile, (const wchar_t *) L"Unknown class %ls", (wchar_t *) name);*/
#define TS_MISSING_NAME(tsFile, token, type) \
  ts_token_syntax_error((const wchar_t *) L"Expecting name for:", tsFile, token ); \
  ts_token_syntax_error_info(tsFile, (const wchar_t *) L"    %ls", (const wchar_t *) type);
#define TS_UNEXPECTED_GLOBAL_TOKEN(tsFile, token, type) \
  ts_token_syntax_error( \
    (const wchar_t *) L"Unexpected `" type "` in global scope", \
    tsFile, token );

#if DEBUG == 1
#define TS_GET_TOKEN_MSG(msg, ...) if (TS_check_log_level(TS_VERBOSITY_INFO) == TRUE) TS_log_to_file(msg, __VA_ARGS__);
#else
#define TS_GET_TOKEN_MSG(msg, ...) ;
#endif

#define TS_TOKEN_BEGIN(type, tsParseData) \
  { \
    TS_MOVE_BY(tsParseData, tsParseData->token); \
    TSParserToken *token = TS_build_parser_token(type, tsParseData); \
    TS_log_to_file(L"-> parsing as %s\n", #type);
#define TS_TOKEN_END(type) \
    tsParseData->parentTSToken = token->parent; \
    TS_log_to_file(L"-> end %s\n", #type); \
    return token; \
  }

typedef struct sTSParseData TSParseData;
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
  TS_CLASS_FIELD = 5,
  TS_CLASS_METHOD = 6,
  TS_EXTENDS = 7,
  TS_IMPLEMENTS = 8,
  TS_FUNCTION = 9,
  TS_FUNCTION_RETURN_TYPE = 10,
  TS_ARROW = 11,
  TS_IF = 12,
  TS_ELSE = 13,
  TS_RETURN = 14,
  TS_DECORATOR = 15,
  TS_DEFAULT = 16,
  TS_SCOPE = 17,
  TS_NEW = 18,
  TS_INLINE_COMMENT = 19,
  TS_MULTILINE_COMMENT = 20,
  TS_CONDITION = 21,
  TS_ARGUMENT = 22,
  TS_CALLER = 23,
  TS_SWITCH = 24,
  TS_CASE = 25,
  TS_BREAK = 26,
  TS_FOR = 27,
  TS_FOR_WITH_CONDITION = 28,
  TS_FOR_IN = 29,
  TS_FOR_OF = 30,
  TS_LOOP_VARIABLES_SECTION = 31,
  TS_LOOP_CONDITION_SECTION = 32,
  TS_LOOP_CHANGE_SECTION = 33,
  TS_OF = 34,
  TS_IN = 35,
  TS_JSON = 36,
  TS_ARRAY = 37,
  TS_STRING = 38,
  TS_STRING_TEMPLATE = 39,
  TS_CALL_ARGUMENTS = 40,
  TS_EXPORT = 41,
  TS_IMPORT = 42,
  TS_IMPORT_FROM = 43,
  TS_IMPORTED_TOKENS = 44,
  TS_INTERFACE = 45,
  TS_BORROW = 46,
  TS_OPERATOR = 47,
  TS_NUMBER = 48,
  TS_JSON_ENTRY = 49,
  TS_UNKNOWN = 0,
} __attribute__ ((__packed__)) TSTokenType;

typedef enum sTSClassParseFlag {
  TS_PARSE_CLASS_MEMBER_NAME = 0x0,
  TS_PARSE_CLASS_MEMBER_TYPE = 0x1,
  TS_PARSE_CLASS_MEMBER_VALUE = 0x2,
  TS_PARSE_CLASS_MEMBER_METHOD_ARGUMENTS = 0x3,
  TS_PARSE_CLASS_MEMBER_METHOD_BODY = 0x4,
  TS_PARSE_CLASS_MEMBER_METHOD_RETURN_TYPE = 0x5,
} __attribute__((__packed__)) TSClassParseFlag;

typedef enum sTSVariableParseFlag {
  TS_PARSE_VARIABLE_NONE = 0,
  TS_PARSE_VARIABLE_NAME = 1,
  TS_PARSE_VARIABLE_TYPE = 1 << 1,
  TS_PARSE_VARIABLE_VALUE = 1 << 2,
} __attribute__((__packed__)) TSVariableParseFlag;

typedef enum eTSForIterationParseFlag {
  TS_PARSE_FOR_VARIABLE = 0,
  TS_PARSE_FOR_ITERATION_KEYWORD = 1 << 0,
  TS_PARSE_FOR_COLLECTION = 1 << 1
} TSForIterationParseFlag;

typedef enum eTSForWithConditionParseFlag {
  TS_PARSE_FOR_VARIABLES = 0,
  TS_PARSE_FOR_CONDITION = 1 << 0,
  TS_PARSE_FOR_CHANGE = 1 << 1,
} TSForWithConditionParseFlag;

typedef enum eTSModifier {
  TS_MODIFIER_NONE = 0,
  TS_MODIFIER_SCOPE = 1 << 0,
  TS_MODIFIER_PRIVATE = 1 << 1,
  TS_MODIFIER_PROTECTED = 1 << 2,
  TS_MODIFIER_PUBLIC = 1 << 3,
  TS_MODIFIER_STATIC = 1 << 4,
  TS_MODIFIER_GETTER = 1 << 4,
  TS_MODIFIER_SETTER = 1 << 6,
} __attribute__ ((__packed__)) TSModifier;

typedef enum eTSParseBracketType {
  TS_PARSE_BRACKET_AS_SCOPE = 1,
  TS_PARSE_BRACKET_AS_JSON = 2,
} __attribute__ ((__packed__)) TSParseBracketType;

typedef enum eTSVariableChild {
  TS_VARIABLE_TYPE = 0,
  TS_VARIABLE_VALUE = 1
} TSVariableChild;

typedef enum eTSJSONEntryType {
  TS_JSON_KEY = 0,
  TS_JSON_VALUE = 1
} TSJSONEntryType;

typedef struct sTSKeyword {
  const wchar_t *str;
  TS_token_build_fn callback;
} TSKeyword;

typedef struct sTSParseData {
  u_int line;
  u_int character;
  const wchar_t *token;
  FILE *stream;
  TSParserToken *parentTSToken;
} TSParseData;

typedef struct sTSParserToken {
  TSTokenType tokenType;
  u_int line;
  u_int character;
  u_int usageCount;
  u_int childrenSize;
  TSParserToken **children;
  TSModifier modifiers;
  TSParserToken *parent;
  union {
    void *data;
    wchar_t *name;
    wchar_t *content;
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

TSParserToken *TS_parse_operator(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_operator(const TSParserToken *token);

TSParserToken *TS_parse_call_arguments(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_call_arguments(const TSParserToken *token);

TSParserToken *TS_parse_interface(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_interface(const TSParserToken *token);

TSParserToken *TS_parse_string(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_string(const TSParserToken *token);

TSParserToken *TS_parse_array(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_array(const TSParserToken *token);

TSParserToken *TS_parse_in(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_in(const TSParserToken *token);

TSParserToken *TS_parse_of(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_of(const TSParserToken *token);

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

void TS_free_function_return_type(const TSParserToken *token);

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

void TS_free_import_from(const TSParserToken *token);

void TS_free_imported_tokens(const TSParserToken *token);

TSParserToken *TS_parse_export(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_export(const TSParserToken *token);

TSParserToken *TS_parse_default(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_default(const TSParserToken *token);

TSParserToken *TS_parse_scope_or_json(TSFile *tsFile, TSParseData *tsParseData);

TSParserToken *TS_parse_json(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_json(const TSParserToken *token);

void TS_free_json_entry(const TSParserToken *token);

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

//TSParserToken *TS_parse_caller(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_caller(const TSParserToken *token);

TSParserToken *TS_parse_for(TSFile *tsFile, TSParseData *tsParseData);

void TS_free_for(const TSParserToken *token);

TSParserToken *__attribute__((__used__))TS_create_borrow(TSParserToken *child, TSParseData *tsParseData);

void TS_free_borrow(const TSParserToken *token);

TSParserToken *TS_parse_ts_token(TSFile *tsFile, TSParseData *data);

void TS_free_tsToken(const TSParserToken *token);

void TS_free_children(const TSParserToken *token);

void TS_free_children_from(const TSParserToken *token, u_long childIndex);

volatile const wchar_t *TS_getToken(FILE *stream) __attribute__((__malloc__));

TSFile *TS_parse_file(const char *fileName);

TSFile *TS_parse_stream(const char *file, FILE *stream);

void TS_free_tsFile(TSFile *tsFile);
