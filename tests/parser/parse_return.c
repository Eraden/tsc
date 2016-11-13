#include "parse_return.h"

START_TEST(parse_valid_return)
  TSFile *tsFile = TS_parse_file("./examples/return/valid.ts");
  ck_assert_uint_eq(tsFile->tokensSize, 5);

  TSParserToken *token = NULL, *ret = NULL, *returned = NULL, *callArgs = NULL, *returnType = NULL, *scope = NULL, *semicolon = NULL;

  token = tsFile->tokens[0];
  ck_assert_eq_ts_function(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 3);
  ck_assert_ptr_ne(token->children, NULL);
  callArgs = token->children[0];
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);
  returnType = token->children[1];
  ck_assert_eq_ts_function_return_type(returnType->tokenType);
  scope = token->children[2];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_ptr_ne(scope->children, NULL);
  ck_assert_uint_eq(scope->childrenSize, 2);
  ret = scope->children[0];
  ck_assert_eq_ts_return(ret->tokenType);
  ck_assert_uint_eq(ret->childrenSize, 0);
  ck_assert_ptr_eq(ret->children, NULL);
  semicolon = scope->children[1];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[1];
  ck_assert_eq_ts_function(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 3);
  ck_assert_ptr_ne(token->children, NULL);
  callArgs = token->children[0];
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);
  returnType = token->children[1];
  ck_assert_eq_ts_function_return_type(returnType->tokenType);
  scope = token->children[2];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_ptr_ne(scope->children, NULL);
  ck_assert_uint_eq(scope->childrenSize, 2);
  ret = scope->children[0];
  ck_assert_eq_ts_return(ret->tokenType);
  ck_assert_uint_eq(ret->childrenSize, 1);
  ck_assert_ptr_ne(ret->children, NULL);
  returned = ret->children[0];
  ck_assert_eq_ts_number(returned->tokenType);
  ck_assert_ptr_ne(returned->name, NULL);
  ck_assert_wstr_eq(returned->name, L"1");
  semicolon = scope->children[1];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[2];
  ck_assert_eq_ts_function(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 3);
  ck_assert_ptr_ne(token->children, NULL);
  callArgs = token->children[0];
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);
  returnType = token->children[1];
  ck_assert_eq_ts_function_return_type(returnType->tokenType);
  scope = token->children[2];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_ptr_ne(scope->children, NULL);
  ck_assert_uint_eq(scope->childrenSize, 2);
  ret = scope->children[0];
  ck_assert_eq_ts_return(ret->tokenType);
  ck_assert_uint_eq(ret->childrenSize, 1);
  ck_assert_ptr_ne(ret->children, NULL);
  returned = ret->children[0];
  ck_assert_eq_ts_json(returned->tokenType);
  semicolon = scope->children[1];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[3];
  ck_assert_eq_ts_function(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 3);
  ck_assert_ptr_ne(token->children, NULL);
  callArgs = token->children[0];
  returnType = token->children[1];
  ck_assert_eq_ts_function_return_type(returnType->tokenType);
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);
  scope = token->children[2];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_ptr_ne(scope->children, NULL);
  ck_assert_uint_eq(scope->childrenSize, 2);
  ret = scope->children[0];
  ck_assert(ret->tokenType == TS_RETURN);
  ck_assert_uint_eq(ret->childrenSize, 1);
  ck_assert_ptr_ne(ret->children, NULL);
  returned = ret->children[0];
  ck_assert_eq_ts_unknown(returned->tokenType);
  ck_assert_ptr_ne(returned->name, NULL);
  ck_assert_wstr_eq(returned->name, L"true");
  semicolon = scope->children[1];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[4];
  ck_assert_eq_ts_function(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 3);
  ck_assert_ptr_ne(token->children, NULL);
  callArgs = token->children[0];
  returnType = token->children[1];
  ck_assert_eq_ts_function_return_type(returnType->tokenType);
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);
  scope = token->children[2];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_ptr_ne(scope->children, NULL);
  ck_assert_uint_eq(scope->childrenSize, 2);
  ret = scope->children[0];
  ck_assert_eq_ts_return(ret->tokenType);
  ck_assert_uint_eq(ret->childrenSize, 1);
  ck_assert_ptr_ne(ret->children, NULL);
  returned = ret->children[0];
  ck_assert_eq_ts_unknown(returned->tokenType);
  ck_assert_ptr_ne(returned->name, NULL);
  ck_assert_wstr_eq(returned->name, L"false");
  semicolon = scope->children[1];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_invalid_export_with_unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/return/without_function");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

void parse_return_keyword_suite(Suite *suite) {
  TCase *tc_return_keyword = tcase_create("Return keyword");

  tcase_add_test(tc_return_keyword, parse_valid_return);
  tcase_add_test(tc_return_keyword, parse_invalid_export_with_unexpected_end_of_stream);

  suite_add_tcase(suite, tc_return_keyword);
}