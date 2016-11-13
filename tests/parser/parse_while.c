#include "parse_while.h"

START_TEST(parse_valid_while)
  TSFile *tsFile = TS_parse_file("./examples/while/valid.ts");

  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 3);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

  TSParserToken *loopToken = NULL, *scope = NULL, *condition = NULL;

  loopToken = tsFile->tokens[0];
  ck_assert_ptr_ne(loopToken, NULL);
  ck_assert_eq_ts_while(loopToken->tokenType);
  ck_assert_uint_eq(loopToken->childrenSize, 2);
  ck_assert_ptr_ne(loopToken->children, NULL);
  condition = loopToken->children[0];
  scope = loopToken->children[1];
  ck_assert_ptr_ne(condition, NULL);
  ck_assert_ptr_ne(scope, NULL);
  ck_assert_eq_ts_condition(condition->tokenType);
  ck_assert_eq_ts_scope(scope->tokenType);

  loopToken = tsFile->tokens[1];
  ck_assert_ptr_ne(loopToken, NULL);
  ck_assert_eq_ts_while(loopToken->tokenType);
  ck_assert_uint_eq(loopToken->childrenSize, 2);
  ck_assert_ptr_ne(loopToken->children, NULL);
  condition = loopToken->children[0];
  scope = loopToken->children[1];
  ck_assert_ptr_ne(condition, NULL);
  ck_assert_ptr_ne(scope, NULL);
  ck_assert_eq_ts_condition(condition->tokenType);
  ck_assert_eq_ts_scope(scope->tokenType);

  loopToken = tsFile->tokens[2];
  ck_assert_ptr_ne(loopToken, NULL);
  ck_assert_eq_ts_while(loopToken->tokenType);
  ck_assert_uint_eq(loopToken->childrenSize, 2);
  ck_assert_ptr_ne(loopToken->children, NULL);
  condition = loopToken->children[0];
  scope = loopToken->children[1];
  ck_assert_ptr_ne(condition, NULL);
  ck_assert_ptr_ne(scope, NULL);
  ck_assert_eq_ts_condition(condition->tokenType);
  ck_assert_eq_ts_scope(scope->tokenType);

  TS_free_ts_file(tsFile);
END_TEST

void parse_while_suite(Suite *suite) {
  TCase *tc_parse_while = tcase_create("Parse while");
  tcase_add_test(tc_parse_while, parse_valid_while);
  suite_add_tcase(suite, tc_parse_while);
}