#include "./borrows.h"

START_TEST(check_borroms)
  TSFile *tsFile = TS_parse_file("./examples/scope/valid.ts");
  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 4);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

  TSParserToken *token = NULL, *scope = NULL, *globalVariable = NULL, *semicolon = NULL;

  globalVariable = tsFile->tokens[0];
  ck_assert_eq_ts_var(globalVariable->tokenType);

  semicolon = tsFile->tokens[1];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[2];
  ck_assert_eq_ts_function(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 3);
  scope = token->children[2];
  ck_assert_eq_ts_scope(scope->tokenType);

  TS_free_tsFile(tsFile);
END_TEST

void
parse_borrows_suite(
    Suite *suite
) {
  TCase *tc_borrows = tcase_create("borrows");
  tcase_add_test(tc_borrows, check_borroms);
  suite_add_tcase(suite, tc_borrows);
}
