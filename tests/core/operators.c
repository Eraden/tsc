#include "./operators.h"

START_TEST(check_valid)
  TSFile *tsFile = TS_parse_file("./examples/operators/valid.ts");
  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 1);
  TSParserToken *scope, *token;
  scope = tsFile->tokens[0];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_uint_eq(scope->childrenSize, 22);
  token = scope->children[0];
  ck_assert_eq_ts_var(token->tokenType);
  token = scope->children[1];
  ck_assert_eq_ts_var(token->tokenType);
  TS_free_tsFile(tsFile);
END_TEST

void parse_operators_suite(Suite *suite) {
  TCase *tc_op = tcase_create("operators");
  tcase_add_test(tc_op, check_valid);
  suite_add_tcase(suite, tc_op);
}