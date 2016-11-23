#include <cts/register.h>
#include "parse_function.h"

static void
validate_function_children(TSParserToken *token) {
  ck_assert_int_eq(token->childrenSize, 3);
  ck_assert_ptr_ne(token->children, NULL);
  TSParserToken *callArgs, *returnType, *body;
  callArgs = token->children[0];
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);
  returnType = token->children[1];
  ck_assert_eq_ts_function_return_type(returnType->tokenType);
  body = token->children[2];
  ck_assert_eq_ts_scope(body->tokenType);
}

START_TEST(parse_valid_functions)
  TSFile *tsFile = TS_parse_file("./examples/functions/valid.ts");

  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 10);

  TSParserToken *token, *arg, *callArgs;

  // example1
  token = tsFile->tokens[0];
  validate_function_children(token);
  ck_assert_eq_ts_function(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"example1");

  // example2
  token = tsFile->tokens[1];
  ck_assert_eq_ts_function(token->tokenType);
  validate_function_children(token);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"example2");
  callArgs = token->children[0];
  arg = callArgs->children[0];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first1");
  TS_check_validate_borrow(arg->children[TS_VARIABLE_TYPE_INDEX], ANY);
//  ck_assert_ts_token_eq();
  ck_assert_eq_ts_undefined(arg->children[TS_VARIABLE_VALUE_INDEX]->tokenType);

  // example3
  token = tsFile->tokens[2];
  ck_assert_eq_ts_function(token->tokenType);
  validate_function_children(token);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"example3");
  callArgs = token->children[0];
  arg = callArgs->children[0];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first2");
  TS_check_validate_borrow(arg->children[TS_VARIABLE_TYPE_INDEX], ANY);
  ck_assert_wstr_eq(arg->children[TS_VARIABLE_VALUE_INDEX]->content, L"1");

  // example4
  token = tsFile->tokens[3];
  ck_assert_eq_ts_function(token->tokenType);
  validate_function_children(token);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"example4");
  callArgs = token->children[0];
  arg = callArgs->children[0];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first3");
  TS_check_validate_borrow(arg->children[TS_VARIABLE_TYPE_INDEX], NUMBER);
  ck_assert_eq_ts_undefined(arg->children[TS_VARIABLE_VALUE_INDEX]->tokenType);

  // example5
  token = tsFile->tokens[4];
  ck_assert_eq_ts_function(token->tokenType);
  validate_function_children(token);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"example5");
  callArgs = token->children[0];
  arg = callArgs->children[0];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first4");
  TS_check_validate_borrow(arg->children[TS_VARIABLE_TYPE_INDEX], NUMBER);
  ck_assert_wstr_eq(arg->children[TS_VARIABLE_VALUE_INDEX]->content, L"234");

  // example6
  token = tsFile->tokens[5];
  ck_assert_eq_ts_function(token->tokenType);
  validate_function_children(token);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"example6");
  callArgs = token->children[0];
  arg = callArgs->children[0];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first5");
  TS_check_validate_borrow(arg->children[TS_VARIABLE_TYPE_INDEX], ANY);
  ck_assert_eq_ts_undefined(arg->children[TS_VARIABLE_VALUE_INDEX]->tokenType);
  arg = callArgs->children[1];
  ck_assert(arg->tokenType);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"second5");
  TS_check_validate_borrow(arg->children[TS_VARIABLE_TYPE_INDEX], ANY);
  ck_assert_eq_ts_undefined(arg->children[TS_VARIABLE_VALUE_INDEX]->tokenType);

  token = tsFile->tokens[6];
  ck_assert_eq_ts_function(token->tokenType);
  validate_function_children(token);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"example7");
  callArgs = token->children[0];
  arg = callArgs->children[0];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first6");
  TS_check_validate_borrow(arg->children[TS_VARIABLE_TYPE_INDEX], ANY);
  ck_assert_wstr_eq(arg->children[TS_VARIABLE_VALUE_INDEX]->content, L"1");
  arg = callArgs->children[1];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"second6");
  TS_check_validate_borrow(arg->children[TS_VARIABLE_TYPE_INDEX], ANY);
  ck_assert_wstr_eq(arg->children[TS_VARIABLE_VALUE_INDEX]->content, L"2");

  // example8
  token = tsFile->tokens[7];
  ck_assert_eq_ts_function(token->tokenType);
  validate_function_children(token);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"example8");
  callArgs = token->children[0];
  arg = callArgs->children[0];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first7");
  TS_check_validate_borrow(arg->children[TS_VARIABLE_TYPE_INDEX], NUMBER);
  ck_assert_eq_ts_undefined(arg->children[TS_VARIABLE_VALUE_INDEX]->tokenType);
  arg = callArgs->children[1];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"second7");
  TS_check_validate_borrow(arg->children[TS_VARIABLE_TYPE_INDEX], OBJECT);
  ck_assert_eq_ts_undefined(arg->children[TS_VARIABLE_VALUE_INDEX]->tokenType);

  // example9
  token = tsFile->tokens[8];
  ck_assert_eq_ts_function(token->tokenType);
  validate_function_children(token);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"example9");
  callArgs = token->children[0];
  arg = callArgs->children[0];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first8");
  TS_check_validate_borrow(arg->children[TS_VARIABLE_TYPE_INDEX], NUMBER);
  ck_assert_wstr_eq(arg->children[TS_VARIABLE_VALUE_INDEX]->content, L"234");
  arg = callArgs->children[1];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"second8");
  TS_check_validate_borrow(arg->children[TS_VARIABLE_TYPE_INDEX], OBJECT);
  ck_assert_eq_ts_json(arg->children[TS_VARIABLE_VALUE_INDEX]->tokenType);

  // example10
  token = tsFile->tokens[9];
  ck_assert_eq_ts_function(token->tokenType);
  validate_function_children(token);
  ck_assert_eq_ts_function_return_type(token->children[1]->tokenType);
  ck_assert_wstr_eq(token->name, L"example10");

  TS_free_ts_file(tsFile);
END_TEST

START_TEST(parse_function_with_missing_return_type)
  TSFile *file = TS_parse_file("./examples/functions/missing_return_type");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_function_with_invalid_return_type)
  TSFile *file = TS_parse_file("./examples/functions/invalid_return_type");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_function_with_invalid_return_type_name)
  TSFile *file = TS_parse_file("./examples/functions/invalid_return_type_name");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_function_with_invalid_arg_without_name)
  TSFile *file = TS_parse_file("./examples/functions/missing_arg_name");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_function_with_invalid_arg_with_missing_type)
  TSFile *file = TS_parse_file("./examples/functions/missing_arg_type");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_function_with_invalid_arg_with_missing_value)
  TSFile *file = TS_parse_file("./examples/functions/missing_arg_value");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_function_with_invalid_arg_skipping_to_next_before_giving_arg_name)
  TSFile *file = TS_parse_file("./examples/functions/missing_first_arg_name");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
END_TEST

void parse_functions_suite(Suite *suite) {
  TCase *tc_functions = tcase_create("Functions");

  tcase_add_test(tc_functions, parse_valid_functions);

  tcase_add_test(tc_functions, parse_function_with_missing_return_type);
  tcase_add_test(tc_functions, parse_function_with_invalid_return_type);
  tcase_add_test(tc_functions, parse_function_with_invalid_return_type_name);

  tcase_add_test(tc_functions, parse_function_with_invalid_arg_without_name);
  tcase_add_test(tc_functions, parse_function_with_invalid_arg_with_missing_type);
  tcase_add_test(tc_functions, parse_function_with_invalid_arg_with_missing_value);
  tcase_add_test(tc_functions, parse_function_with_invalid_arg_skipping_to_next_before_giving_arg_name);

  suite_add_tcase(suite, tc_functions);
}