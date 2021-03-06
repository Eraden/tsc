#include "parse_new.h"

START_TEST(parse_valid_new_keyword)
  TSFile *tsFile = TS_parse_file("./examples/new/valid.ts");
  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 6);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

  TSParserToken *token = NULL, *classToken = NULL, *arg = NULL, *callArgs = NULL, *semicolon = NULL;

  token = tsFile->tokens[0];
  ck_assert_eq_ts_new(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  classToken = token->children[0];
  TS_check_validate_borrow(classToken, STRING);

  semicolon = tsFile->tokens[1];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[2];
  ck_assert_eq_ts_new(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  classToken = token->children[0];
  TS_check_validate_borrow(classToken, FUNCTION);
  callArgs = token->children[1];
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);

  semicolon = tsFile->tokens[3];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[4];
  ck_assert_eq_ts_new(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  classToken = token->children[0];
  TS_check_validate_borrow(classToken, ARRAY);
  callArgs = token->children[1];
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);
  arg = callArgs->children[0];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"1");
  arg = callArgs->children[1];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"2");
  arg = callArgs->children[2];
  ck_assert_eq_ts_argument(arg->tokenType);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"3");

  semicolon = tsFile->tokens[5];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  TS_free_ts_file(tsFile);
END_TEST

START_TEST(parse_invalid_new_keyword)
  TSFile *tsFile = TS_parse_file("./examples/new/unexpected_new_line");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_ts_file(tsFile);
END_TEST

START_TEST(parse_invalid_new_keyword_with_new_line)
  TSFile *tsFile = TS_parse_file("./examples/new/unexpected_end_of_stream");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_ts_file(tsFile);
END_TEST

START_TEST(parse_invalid_new_keyword_with_end)
  TSFile *tsFile = TS_parse_file("./examples/new/unexpected_colon");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_ts_file(tsFile);
END_TEST

void parse_new_suite(Suite *suite) {
  TCase *tc_new_keyword = tcase_create("Parse new keyword");

  tcase_add_test(tc_new_keyword, parse_valid_new_keyword);

  tcase_add_test(tc_new_keyword, parse_invalid_new_keyword);
  tcase_add_test(tc_new_keyword, parse_invalid_new_keyword_with_new_line);
  tcase_add_test(tc_new_keyword, parse_invalid_new_keyword_with_end);

  suite_add_tcase(suite, tc_new_keyword);
}
