#pragma once

#include <cts/parser.h>

static char *
__attribute__((__used__))
_TS_type_to_string(const TSTokenType type) {
  switch (type) {
    case TS_UNKNOWN:
      return "TS_UNKNOWN";
    case TS_THIS:
      return "TS_THIS";
    case TS_SUPER:
      return "TS_SUPER";
    case TS_FALSE:
      return "TS_FALSE";
    case TS_TRUE:
      return "TS_TRUE";
    case TS_UNDEFINED:
      return "TS_UNDEFINED";
    case TS_NULL:
      return "TS_NULL";
    case TS_VAR:
      return "TS_VAR";
    case TS_LET:
      return "TS_LET";
    case TS_CONST:
      return "TS_CONST";
    case TS_SEMICOLON:
      return "TS_SEMICOLON";
    case TS_COLON:
      return "TS_COLON";
    case TS_CLASS:
      return "TS_CLASS";
    case TS_CLASS_FIELD:
      return "TS_CLASS_FIELD";
    case TS_CLASS_METHOD:
      return "TS_CLASS_METHOD";
    case TS_EXTENDS:
      return "TS_EXTENDS";
    case TS_IMPLEMENTS:
      return "TS_IMPLEMENTS";
    case TS_FUNCTION:
      return "TS_FUNCTION";
    case TS_FUNCTION_RETURN_TYPE:
      return "TS_FUNCTION_RETURN_TYPE";
    case TS_ARROW:
      return "TS_ARROW";
    case TS_IF:
      return "TS_IF";
    case TS_ELSE:
      return "TS_ELSE";
    case TS_RETURN:
      return "TS_RETURN";
    case TS_DECORATOR:
      return "TS_DECORATOR";
    case TS_DEFAULT:
      return "TS_DEFAULT";
    case TS_SCOPE:
      return "TS_SCOPE";
    case TS_NEW:
      return "TS_NEW";
    case TS_INLINE_COMMENT:
      return "TS_INLINE_COMMENT";
    case TS_MULTILINE_COMMENT:
      return "TS_MULTILINE_COMMENT";
    case TS_CONDITION:
      return "TS_CONDITION";
    case TS_ARGUMENT:
      return "TS_ARGUMENT";
    case TS_CALLER:
      return "TS_CALLER";
    case TS_SWITCH:
      return "TS_SWITCH";
    case TS_SWITCH_BODY:
      return "TS_SWITCH_BODY";
    case TS_CASE:
      return "TS_CASE";
    case TS_BREAK:
      return "TS_BREAK";
    case TS_DO:
      return "TS_DO";
    case TS_WHILE:
      return "TS_WHILE";
    case TS_FOR:
      return "TS_FOR";
    case TS_FOR_WITH_CONDITION:
      return "TS_FOR_WITH_CONDITION";
    case TS_FOR_IN:
      return "TS_FOR_IN";
    case TS_FOR_OF:
      return "TS_FOR_OF";
    case TS_LOOP_VARIABLES_SECTION:
      return "TS_LOOP_VARIABLES_SECTION";
    case TS_LOOP_CONDITION_SECTION:
      return "TS_LOOP_CONDITION_SECTION";
    case TS_LOOP_CHANGE_SECTION:
      return "TS_LOOP_CHANGE_SECTION";
    case TS_OF:
      return "TS_OF";
    case TS_IN:
      return "TS_IN";
    case TS_JSON:
      return "TS_JSON";
    case TS_JSON_ENTRY:
      return "TS_JSON_ENTRY";
    case TS_ARRAY:
      return "TS_ARRAY";
    case TS_STRING:
      return "TS_STRING";
    case TS_STRING_TEMPLATE:
      return "TS_STRING_TEMPLATE";
    case TS_CALL_ARGUMENTS:
      return "TS_CALL_ARGUMENTS";
    case TS_EXPORT:
      return "TS_EXPORT";
    case TS_IMPORT:
      return "TS_IMPORT";
    case TS_IMPORT_FROM:
      return "TS_IMPORT_FROM";
    case TS_IMPORTED_TOKENS:
      return "TS_IMPORTED_TOKENS";
    case TS_INTERFACE:
      return "TS_INTERFACE";
    case TS_INTERFACE_BODY:
      return "TS_INTERFACE_BODY";
    case TS_BORROW:
      return "TS_BORROW";
    case TS_OPERATOR:
      return "TS_OPERATOR";
    case TS_NUMBER:
      return "TS_NUMBER";
    case TS_GROUP:
      return "TS_GROUP";
    case TS_SPREAD:
      return "TS_SPREAD";
    default:
      return " ((unknown type)) ";
  }
}

#define _ck_assert_token_type(X, OP, Y) { \
  const TSTokenType a = X; \
  const TSTokenType b = Y;\
  ck_assert_msg( \
    a == b, "Assertion '%s' failed: %s == %i, %s == %i", \
    #X" "#OP" "#Y, \
    _TS_type_to_string(a), a,\
    _TS_type_to_string(b), b\
  ); \
}

#define ck_assert_tsType_eq(X, Y)  _ck_assert_token_type(X, ==, Y)

#define ck_assert_eq_ts_this(type) ck_assert_tsType_eq(type, TS_THIS)
#define ck_assert_eq_ts_super(type) ck_assert_tsType_eq(type, TS_SUPER)
#define ck_assert_eq_ts_false(type) ck_assert_tsType_eq(type, TS_FALSE)
#define ck_assert_eq_ts_true(type) ck_assert_tsType_eq(type, TS_TRUE)
#define ck_assert_eq_ts_undefined(type) ck_assert_tsType_eq(type, TS_UNDEFINED)
#define ck_assert_eq_ts_null(type) ck_assert_tsType_eq(type, TS_NULL)
#define ck_assert_eq_ts_var(type) ck_assert_tsType_eq(type, TS_VAR)
#define ck_assert_eq_ts_let(type) ck_assert_tsType_eq(type, TS_LET)
#define ck_assert_eq_ts_const(type) ck_assert_tsType_eq(type, TS_CONST)
#define ck_assert_eq_ts_semicolon(type) ck_assert_tsType_eq(type, TS_SEMICOLON)
#define ck_assert_eq_ts_class(type) ck_assert_tsType_eq(type, TS_CLASS)
#define ck_assert_eq_ts_function(type) ck_assert_tsType_eq(type, TS_FUNCTION)
#define ck_assert_eq_ts_function_return_type(type) ck_assert_tsType_eq(type, TS_FUNCTION_RETURN_TYPE)
#define ck_assert_eq_ts_arrow(type) ck_assert_tsType_eq(type, TS_ARROW)
#define ck_assert_eq_ts_if(type) ck_assert_tsType_eq(type, TS_IF)
#define ck_assert_eq_ts_else(type) ck_assert_tsType_eq(type, TS_ELSE)
#define ck_assert_eq_ts_return(type) ck_assert_tsType_eq(type, TS_RETURN)
#define ck_assert_eq_ts_decorator(type) ck_assert_tsType_eq(type, TS_DECORATOR)
#define ck_assert_eq_ts_import(type) ck_assert_tsType_eq(type, TS_IMPORT)
#define ck_assert_eq_ts_export(type) ck_assert_tsType_eq(type, TS_EXPORT)
#define ck_assert_eq_ts_default(type) ck_assert_tsType_eq(type, TS_DEFAULT)
#define ck_assert_eq_ts_scope(type) ck_assert_tsType_eq(type, TS_SCOPE)
#define ck_assert_eq_ts_json(type) ck_assert_tsType_eq(type, TS_JSON)
#define ck_assert_eq_ts_json_entry(type) ck_assert_tsType_eq(type, TS_JSON_ENTRY)
#define ck_assert_eq_ts_extends(type) ck_assert_tsType_eq(type, TS_EXTENDS)
#define ck_assert_eq_ts_implements(type) ck_assert_tsType_eq(type, TS_IMPLEMENTS)
#define ck_assert_eq_ts_interface(type) ck_assert_tsType_eq(type, TS_INTERFACE)
#define ck_assert_eq_ts_new(type) ck_assert_tsType_eq(type, TS_NEW)
#define ck_assert_eq_ts_class_field(type) ck_assert_tsType_eq(type, TS_CLASS_FIELD)
#define ck_assert_eq_ts_class_method(type) ck_assert_tsType_eq(type, TS_CLASS_METHOD)
#define ck_assert_eq_ts_inline_comment(type) ck_assert_tsType_eq(type, TS_INLINE_COMMENT)
#define ck_assert_eq_ts_multiline_comment(type) ck_assert_tsType_eq(type, TS_MULTILINE_COMMENT)
#define ck_assert_eq_ts_condition(type) ck_assert_tsType_eq(type, TS_CONDITION)
#define ck_assert_eq_ts_argument(type) ck_assert_tsType_eq(type, TS_ARGUMENT)
#define ck_assert_eq_ts_caller(type) ck_assert_tsType_eq(type, TS_CALLER)
#define ck_assert_eq_ts_switch(type) ck_assert_tsType_eq(type, TS_SWITCH)
#define ck_assert_eq_ts_switch_body(type) ck_assert_tsType_eq(type, TS_SWITCH_BODY)
#define ck_assert_eq_ts_case(type) ck_assert_tsType_eq(type, TS_CASE)
#define ck_assert_eq_ts_break(type) ck_assert_tsType_eq(type, TS_BREAK)
#define ck_assert_eq_ts_do(type) ck_assert_tsType_eq(type, TS_DO)
#define ck_assert_eq_ts_while(type) ck_assert_tsType_eq(type, TS_WHILE)
#define ck_assert_eq_ts_for(type) ck_assert_tsType_eq(type, TS_FOR)
#define ck_assert_eq_ts_for_with_condition(type) ck_assert_tsType_eq(type, TS_FOR_WITH_CONDITION)
#define ck_assert_eq_ts_for_in(type) ck_assert_tsType_eq(type, TS_FOR_IN)
#define ck_assert_eq_ts_for_of(type) ck_assert_tsType_eq(type, TS_FOR_OF)
#define ck_assert_eq_ts_loop_variables_section(type) ck_assert_tsType_eq(type, TS_LOOP_VARIABLES_SECTION);
#define ck_assert_eq_ts_loop_condition_section(type) ck_assert_tsType_eq(type, TS_LOOP_CONDITION_SECTION);
#define ck_assert_eq_ts_loop_change_section(type) ck_assert_tsType_eq(type, TS_LOOP_CHANGE_SECTION);
#define ck_assert_eq_ts_unknown(type) ck_assert_tsType_eq(type, TS_UNKNOWN)
#define ck_assert_eq_ts_call_arguments(type) ck_assert_tsType_eq(type, TS_CALL_ARGUMENTS)
#define ck_assert_eq_ts_borrow(type) ck_assert_tsType_eq(type, TS_BORROW)
#define ck_assert_eq_ts_operator(type) ck_assert_tsType_eq(type, TS_OPERATOR)
#define ck_assert_eq_ts_string(type) ck_assert_tsType_eq(type, TS_STRING)
#define ck_assert_eq_ts_number(type) ck_assert_tsType_eq(type, TS_NUMBER)
#define ck_assert_eq_ts_spread(type) ck_assert_tsType_eq(type, TS_SPREAD)
