#include "parse_multiline_comment.h"

START_TEST(parse_valid_multiline_comment)
  TSFile *tsFile = TS_parse_file("./examples/multiline_comment/valid.ts");
  ck_assert_uint_eq(tsFile->tokensSize, 3);

  TSParserToken *token;

  token = tsFile->tokens[0];
  ck_assert(token->tokenType == TS_MULTILINE_COMMENT);
  ck_assert_uint_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"\nNormal comment\n");

  token = tsFile->tokens[1];
  ck_assert(token->tokenType == TS_MULTILINE_COMMENT);
  ck_assert_uint_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"*\nDoc comment\n");

  token = tsFile->tokens[2];
  ck_assert(token->tokenType == TS_MULTILINE_COMMENT);
  ck_assert_uint_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L" Single line comment ");

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_miltiline_comment_with_no_ending)
  TS_parse_file("./examples/multiline_comment/missing_ending");
END_TEST

void parse_multiline_comment_suite(Suite *suite) {
  TCase *tc_multiline = tcase_create("Parse multiline comment");

  tcase_add_test(tc_multiline, parse_valid_multiline_comment);

  TCase *tc_multiline_failure = tcase_create("Parse multiline comment failure");
  tcase_add_exit_test(tc_multiline_failure, parse_miltiline_comment_with_no_ending, TS_PARSE_FAILURE_CODE);

  suite_add_tcase(suite, tc_multiline);
  suite_add_tcase(suite, tc_multiline_failure);
}