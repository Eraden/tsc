#include "parse_new.h"

START_TEST(parse_valid_new_keyword)
  TSFile *tsFile = TS_parse_file("./examples/new/valid.ts");
  ck_assert_uint_eq(tsFile->tokensSize, 3);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

  TSParserToken *token, *caller, *arg;

  token = tsFile->tokens[0];
  ck_assert(token->tokenType == TS_NEW);
  ck_assert_uint_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  caller = token->children[0];
  ck_assert(caller->tokenType == TS_CALLER);
  ck_assert_ptr_ne(caller->name, NULL);
  ck_assert_wstr_eq(caller->name, L"String");
  ck_assert_uint_eq(caller->childrenSize, 0);
  ck_assert_ptr_eq(caller->children, NULL);

  token = tsFile->tokens[1];
  ck_assert(token->tokenType == TS_NEW);
  ck_assert_uint_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  caller = token->children[0];
  ck_assert(caller->tokenType == TS_CALLER);
  ck_assert_ptr_ne(caller->name, NULL);
  ck_assert_wstr_eq(caller->name, L"Function");
  ck_assert_uint_eq(caller->childrenSize, 0);
  ck_assert_ptr_eq(caller->children, NULL);

  token = tsFile->tokens[2];
  ck_assert(token->tokenType == TS_NEW);
  ck_assert_uint_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  caller = token->children[0];
  ck_assert(caller->tokenType == TS_CALLER);
  ck_assert_ptr_ne(caller->name, NULL);
  ck_assert_wstr_eq(caller->name, L"Array");
  ck_assert_uint_eq(caller->childrenSize, 3);
  ck_assert_ptr_ne(caller->children, NULL);
  arg = caller->children[0];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_ptr_ne(arg->variableData, NULL);
  ck_assert_ptr_ne(arg->variableData->name, NULL);
  ck_assert_wstr_eq(arg->variableData->name, L"1");
  arg = caller->children[1];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_ptr_ne(arg->variableData, NULL);
  ck_assert_ptr_ne(arg->variableData->name, NULL);
  ck_assert_wstr_eq(arg->variableData->name, L"2");
  arg = caller->children[2];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_ptr_ne(arg->variableData, NULL);
  ck_assert_ptr_ne(arg->variableData->name, NULL);
  ck_assert_wstr_eq(arg->variableData->name, L"3");

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_invalid_new_keyword)
  TS_parse_file("./examples/new/unexpected_new_line");
END_TEST

START_TEST(parse_invalid_new_keyword_with_new_line)
  TS_parse_file("./examples/new/unexpected_end_of_stream");
END_TEST

START_TEST(parse_invalid_new_keyword_with_end)
  TS_parse_file("./examples/new/unexpected_colon");
END_TEST

void parse_new_suite(Suite *suite) {
  TCase *tc_new_keyword = tcase_create("Parse new keyword");

  tcase_add_test(tc_new_keyword, parse_valid_new_keyword);

  tcase_add_exit_test(tc_new_keyword, parse_invalid_new_keyword, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_new_keyword, parse_invalid_new_keyword_with_new_line, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_new_keyword, parse_invalid_new_keyword_with_end, TS_PARSE_FAILURE_CODE);

  suite_add_tcase(suite, tc_new_keyword);
}