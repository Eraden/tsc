#pragma once

#include <cts/file.h>
#include <cts/sys.h>
#include <cts/log.h>

#define TS_CODE_EVAL "(code eval)"
#define TS_TOKEN_SECTION_BEGIN(token) TS_log_to_file(L"    -> parsing as %s\n", token);
#define TS_TOKEN_SECTION_END(token) TS_log_to_file(L"    -> end %s\n", token);
#define TS_STRING_END 1
#define TS_NEW_TOKEN calloc(sizeof(TSParserToken), 1)
#define TS_LOOP_SANITY_CHECK(file) if (file->sanity != TS_FILE_VALID) break; // TS_log_to_file(L"Loop in %s:%i\n", __FILE__, __LINE__);
#define TS_MOVE_BY(tsFile, tok) { u_long m = wcslen(tok); tsFile->parse.character += m; }
#define TS_NEW_LINE(tsFile, tok) { u_long m = wcslen(tok); tsFile->parse.character = 0; tsFile->parse.line += 1; }

#define TS_UNEXPECTED_END_OF_STREAM(file, token, type) TS_token_syntax_error( \
  (const wchar_t *) L"Unexpected end of stream while parsing `" type "`", \
  file, token )
#define TS_UNEXPECTED_TOKEN(tsFile, token, tok, type) TS_token_syntax_error( \
  (const wchar_t *) L"Unexpected token while parsing `" type "`", tsFile, token, tok ); \
  TS_token_syntax_error_info(tsFile, (const wchar_t *) L"invalid token: \"%ls\"", (wchar_t *) tok);
#define TS_UNKNOWN_TYPE(tsFile, token, name) TS_token_syntax_error( \
  (const wchar_t *) L"Unknown type", tsFile, token ); \
  TS_token_syntax_error_info(tsFile, (const wchar_t *) L"%ls", (wchar_t *) name);
#define TS_MISSING_NAME(tsFile, token, type) \
  TS_token_syntax_error((const wchar_t *) L"Expecting name for:", tsFile, token ); \
  TS_token_syntax_error_info(tsFile, (const wchar_t *) L"    %ls", (const wchar_t *) type);
#define TS_UNEXPECTED_GLOBAL_TOKEN(tsFile, token, type) \
  TS_token_syntax_error((const wchar_t *) L"Unexpected `" type "` in global scope", tsFile, token );

#if DEBUG == 1
#define TS_GET_TOKEN_MSG(msg, ...) if (TS_check_log_level(TS_VERBOSITY_INFO) == TRUE) TS_log_to_file(msg, __VA_ARGS__);
#else
#define TS_GET_TOKEN_MSG(msg, ...) ;
#endif

#define TS_TOKEN_BEGIN(type, tsFile) \
  { \
    TSParserToken *token = TS_build_parser_token(type, tsFile); \
    TS_MOVE_BY(tsFile, tsFile->parse.token); \
    TS_log_to_file(L"-> parsing as %s\n", #type);
#define TS_TOKEN_END(type) \
    tsFile->parse.parentTSToken = token->parent; \
    TS_log_to_file(L"-> end %s\n", #type); \
    return token; \
  }

#define TS_EACH_CHILD(token) \
  unsigned int childIndex = token->childrenSize; \
  TSParserToken **children = token->children; \
  while (childIndex) { \
    childIndex -= 1; \
    TSParserToken *current = children[0];

#define TS_END_EACH \
    children += 1; \
  }

#define TS_INTERFACE_METHOD_ARGUMENTS_INDEX 0
#define TS_INTERFACE_METHOD_RETURN_TYPE_INDEX 1

#define TS_FUNCTION_CALL_ARGS_INDEX 0
#define TS_FUNCTION_RETURN_TYPE_INDEX 1
#define TS_FUNCTION_BODY_INDEX 2

#define TS_CLASS_METHOD_CALL_ARGS_INDEX 0
#define TS_CLASS_METHOD_RETURN_TYPE_INDEX 1
#define TS_CLASS_METHOD_BODY_INDEX 2

#define TS_SWITCH_CONDITIONS_INDEX 0
#define TS_SWITCH_BODY_INDEX 1

#define TS_VARIABLE_TYPE_INDEX 0
#define TS_VARIABLE_VALUE_INDEX 1

#define TS_OPERATOR_A_INDEX 0
#define TS_OPERATOR_B_INDEX 1

typedef struct sTSKeyword TSKeyword;

typedef TSParserToken *(*TS_token_build_fn)(TSFile *tsFile);

typedef enum eTSTokenType {
  TS_UNKNOWN = 0,
  TS_UNDEFINED,
  TS_NULL,
  TS_THIS,
  TS_SUPER,
  TS_TRUE,
  TS_FALSE,
  TS_VAR,
  TS_LET,
  TS_CONST,
  TS_SEMICOLON,
  TS_COLON,
  TS_CLASS,
  TS_CLASS_FIELD,
  TS_CLASS_METHOD,
  TS_EXTENDS,
  TS_IMPLEMENTS,
  TS_FUNCTION,
  TS_FUNCTION_RETURN_TYPE,
  TS_ARROW,
  TS_IF,
  TS_ELSE,
  TS_RETURN,
  TS_DECORATOR,
  TS_DEFAULT,
  TS_SCOPE,
  TS_NEW,
  TS_INLINE_COMMENT,
  TS_MULTILINE_COMMENT,
  TS_CONDITION,
  TS_ARGUMENT,
  TS_CALLER,
  TS_SWITCH,
  TS_SWITCH_BODY,
  TS_CASE,
  TS_BREAK,
  TS_FOR,
  TS_FOR_WITH_CONDITION,
  TS_FOR_IN,
  TS_FOR_OF,
  TS_WHILE,
  TS_DO,
  TS_LOOP_VARIABLES_SECTION,
  TS_LOOP_CONDITION_SECTION,
  TS_LOOP_CHANGE_SECTION,
  TS_OF,
  TS_IN,
  TS_JSON,
  TS_JSON_ENTRY,
  TS_ARRAY,
  TS_STRING,
  TS_STRING_TEMPLATE,
  TS_CALL_ARGUMENTS,
  TS_EXPORT,
  TS_IMPORT,
  TS_IMPORT_FROM,
  TS_IMPORTED_TOKENS,
  TS_INTERFACE,
  TS_INTERFACE_BODY,
  TS_BORROW,
  TS_OPERATOR,
  TS_NUMBER,
  TS_GROUP,
  TS_NAMESPACE,
  TS_SPREAD,
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
  TS_MODIFIER_GETTER = 1 << 5,
  TS_MODIFIER_SETTER = 1 << 6,
} __attribute__ ((__packed__)) TSModifier;

typedef enum eTSParseBracketType {
  TS_PARSE_BRACKET_AS_SCOPE = 1,
  TS_PARSE_BRACKET_AS_JSON = 2,
} __attribute__ ((__packed__)) TSParseBracketType;

typedef enum eTSJSONEntryType {
  TS_JSON_KEY = 0,
  TS_JSON_VALUE = 1
} TSJSONEntryType;

typedef enum eTSExperimental {
  TS_ENABLE_EXPERIMENTAL = 1,
  TS_DISABLE_EXPERIMENTAL = 0
} TSExperimental;

TSExperimental __attribute__((__used__)) TS_experimentalFlag;

typedef struct sTSKeyword {
  const wchar_t *str;
  TS_token_build_fn callback;
} TSKeyword;

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

typedef struct sTSParseOperatorData {
  TSParserToken *token;
  TSParserToken *parent;
  TSParserToken *prev;
  TSParserToken *next;
  unsigned int lastIndex;
  unsigned char done;
  const wchar_t *tok;
} TSParseOperatorData;

void TS_append_ts_parser_token(TSFile *tsFile, TSParserToken *token);

unsigned char TS_is_keyword(const wchar_t *str);

TSParserToken *TS_build_parser_token(TSTokenType tokenType, TSFile *tsFile);

void TS_put_back(FILE *stream, volatile const wchar_t *value);

unsigned char TS_name_is_valid(const wchar_t *name);

void TS_push_child(TSParserToken *token, TSParserToken *child);

void TS_free_unknown(const TSParserToken *token);

TSParserToken *TS_create_undefined(TSFile *tsFile);

TSParserToken *TS_parse_operator_advanced(TSFile *tsFile);

void TS_free_operator(const TSParserToken *token);

TSParserToken *TS_parse_spread(TSFile *tsFile);

void TS_free_spread(const TSParserToken *token);

TSParserToken *TS_parse_super(TSFile *tsFile);

void TS_free_super(const TSParserToken *token);

TSParserToken *TS_parse_group(TSFile *tsFile);

void TS_free_group(const TSParserToken *token);

TSParserToken *TS_parse_call_arguments(TSFile *tsFile);

void TS_free_call_arguments(const TSParserToken *token);

TSParserToken *TS_parse_interface(TSFile *tsFile);

void TS_free_interface(const TSParserToken *token);

void TS_free_interface_body(const TSParserToken *token);

TSParserToken *TS_parse_string(TSFile *tsFile);

void TS_free_string(const TSParserToken *token);

TSParserToken *TS_parse_array(TSFile *tsFile);

void TS_free_array(const TSParserToken *token);

TSParserToken *TS_parse_in(TSFile *tsFile);

void TS_free_in(const TSParserToken *token);

TSParserToken *TS_parse_of(TSFile *tsFile);

void TS_free_of(const TSParserToken *token);

TSParserToken *TS_parse_condition(TSFile *tsFile);

void TS_free_condition(const TSParserToken *token);

TSParserToken *TS_parse_switch(TSFile *tsFile);

void TS_free_switch(const TSParserToken *token);

void TS_free_switch_body(const TSParserToken *token);

TSParserToken *TS_parse_case(TSFile *tsFile);

void TS_free_case(const TSParserToken *token);

TSParserToken *TS_parse_break(TSFile *tsFile);

void TS_free_break(const TSParserToken *token);

TSParserToken *TS_parse_argument(TSFile *tsFile);

void TS_free_argument(const TSParserToken *token);

TSParserToken *TS_parse_var(TSFile *tsFile);

void TS_free_var(const TSParserToken *token);

TSParserToken *TS_parse_let(TSFile *tsFile);

void TS_free_let(const TSParserToken *token);

TSParserToken *TS_parse_const(TSFile *tsFile);

void TS_free_const(const TSParserToken *token);

TSParserToken *TS_parse_class(TSFile *tsFile);

void TS_free_class(const TSParserToken *token);

void TS_free_class_field(const TSParserToken *token);

void TS_free_class_method(const TSParserToken *token);

TSParserToken *TS_parse_function(TSFile *tsFile);

void TS_free_function(const TSParserToken *token);

void TS_free_function_return_type(const TSParserToken *token);

TSParserToken *TS_parse_arrow(TSFile *tsFile);

void TS_free_arrow(const TSParserToken *token);

TSParserToken *TS_parse_if(TSFile *tsFile);

void TS_free_if(const TSParserToken *token);

TSParserToken *TS_parse_else(TSFile *tsFile);

void TS_free_else(const TSParserToken *token);

TSParserToken *TS_parse_decorator(TSFile *tsFile);

void TS_free_decorator(const TSParserToken *token);

TSParserToken *TS_parse_return(TSFile *tsFile);

void TS_free_return(const TSParserToken *token);

TSParserToken *TS_parse_import(TSFile *tsFile);

void TS_free_import(const TSParserToken *token);

void TS_free_import_from(const TSParserToken *token);

void TS_free_imported_tokens(const TSParserToken *token);

TSParserToken *TS_parse_export(TSFile *tsFile);

void TS_free_export(const TSParserToken *token);

TSParserToken *TS_parse_default(TSFile *tsFile);

void TS_free_default(const TSParserToken *token);

TSParserToken *TS_parse_scope_or_json(TSFile *tsFile);

TSParserToken *TS_parse_json(TSFile *tsFile);

void TS_free_json(const TSParserToken *token);

void TS_free_json_entry(const TSParserToken *token);

TSParserToken *TS_parse_scope(TSFile *tsFile);

void TS_free_scope(const TSParserToken *token);

TSParserToken *TS_parse_extends(TSFile *tsFile);

void TS_free_extends(const TSParserToken *token);

TSParserToken *TS_parse_implements(TSFile *tsFile);

void TS_free_implements(const TSParserToken *token);

TSParserToken *TS_parse_new(TSFile *tsFile);

void TS_free_new(const TSParserToken *token);

TSParserToken *TS_parse_inline_comment(TSFile *tsFile);

void TS_free_inline_comment(const TSParserToken *token);

TSParserToken *TS_parse_multiline_comment(TSFile *tsFile);

void TS_free_multiline_comment(const TSParserToken *token);

//TSParserToken *TS_parse_caller(TSFile *tsFile);

void TS_free_caller(const TSParserToken *token);

TSParserToken *TS_parse_for(TSFile *tsFile);

void TS_free_for(const TSParserToken *token);

TSParserToken *TS_parse_while(TSFile *tsFile);

void TS_free_while(const TSParserToken *token);

TSParserToken *TS_parse_do(TSFile *tsFile);

void TS_free_do(const TSParserToken *token);

TSParserToken *TS_parse_namespace(TSFile *tsFile);

void TS_free_namespace(const TSParserToken *token);

TSParserToken *TS_create_borrow(TSParserToken *child, TSFile *tsFile);

void TS_free_borrow(const TSParserToken *token);

TSParserToken *TS_parse_ts_token(TSFile *tsFile);

void TS_rollback_token(TSFile *tsFile, TSParserToken *token);

void TS_free_ts_token(const TSParserToken *token);

void TS_free_children(const TSParserToken *token);

void TS_free_children_from(const TSParserToken *token, u_long childIndex);

volatile const wchar_t *TS_get_token(FILE *stream) __attribute__((__malloc__));

TSFile *TS_parse_file(const char *fileName);

TSFile *TS_parse_stream(const char *file, FILE *stream);

unsigned char TS_is_embedded_in(TSParserToken *token, TSTokenType type);
