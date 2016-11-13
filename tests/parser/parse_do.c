#include "parse_do.h"

START_TEST(parse_valid_do)
  TSFile *tsFile = TS_parse_file("./examples/do/valid.ts");

  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 4);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

  TSParserToken *loopToken = NULL, *scope = NULL, *whileToken = NULL, *semicolon = NULL;

  loopToken = tsFile->tokens[0];
  ck_assert_ptr_ne(loopToken, NULL);
  ck_assert_eq_ts_do(loopToken->tokenType);
  ck_assert_uint_eq(loopToken->childrenSize, 2);
  ck_assert_ptr_ne(loopToken->children, NULL);
  scope = loopToken->children[0];
  whileToken = loopToken->children[1];
  ck_assert_ptr_ne(whileToken, NULL);
  ck_assert_ptr_ne(scope, NULL);
  ck_assert_eq_ts_while(whileToken->tokenType);
  ck_assert_eq_ts_scope(scope->tokenType);

  loopToken = tsFile->tokens[1];
  ck_assert_ptr_ne(loopToken, NULL);
  ck_assert_eq_ts_do(loopToken->tokenType);
  ck_assert_uint_eq(loopToken->childrenSize, 2);
  ck_assert_ptr_ne(loopToken->children, NULL);
  scope = loopToken->children[0];
  whileToken = loopToken->children[1];
  ck_assert_ptr_ne(whileToken, NULL);
  ck_assert_ptr_ne(scope, NULL);
  ck_assert_eq_ts_while(whileToken->tokenType);
  ck_assert_eq_ts_scope(scope->tokenType);

  semicolon = tsFile->tokens[2];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  loopToken = tsFile->tokens[3];
  ck_assert_ptr_ne(loopToken, NULL);
  ck_assert_eq_ts_do(loopToken->tokenType);
  ck_assert_uint_eq(loopToken->childrenSize, 2);
  ck_assert_ptr_ne(loopToken->children, NULL);
  scope = loopToken->children[0];
  whileToken = loopToken->children[1];
  ck_assert_ptr_ne(whileToken, NULL);
  ck_assert_ptr_ne(scope, NULL);
  ck_assert_eq_ts_while(whileToken->tokenType);
  ck_assert_eq_ts_scope(scope->tokenType);

  TS_free_tsFile(tsFile);
END_TEST

void parse_do_suite(Suite *suite) {
  TCase *tc_parse_do = tcase_create("Parse do");
  tcase_add_test(tc_parse_do, parse_valid_do);
  suite_add_tcase(suite, tc_parse_do);
}