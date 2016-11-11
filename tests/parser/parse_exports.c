#include "./parse_exports.h"

START_TEST(parse_valid_export)
  TSFile *tsFile = TS_parse_file("./examples/export/valid.ts");
  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 6);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

  TSParserToken *token = NULL, *exported = NULL, *semicolon = NULL;

  token = tsFile->tokens[0];
  ck_assert_eq_ts_export(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 1);
  exported = token->children[0];
  ck_assert_eq_ts_function(exported->tokenType);

  token = tsFile->tokens[1];
  ck_assert_eq_ts_export(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 1);
  exported = token->children[0];
  ck_assert_eq_ts_var(exported->tokenType);

  semicolon = tsFile->tokens[2];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[3];
  ck_assert_eq_ts_export(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 1);
  exported = token->children[0];
  ck_assert_eq_ts_const(exported->tokenType);

  semicolon = tsFile->tokens[4];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[5];
  ck_assert_eq_ts_export(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 1);
  exported = token->children[0];
  ck_assert_eq_ts_class(exported->tokenType);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_invalid_export_with_newline)
  TSFile *tsFile = TS_parse_file("./examples/export/unexpected_new_line");
  ck_assert_tsFile_syntax_error(tsFile);
  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_invalid_export_with_unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/export/unexpected_end_of_stream");
  ck_assert_tsFile_syntax_error(tsFile);
  TS_free_tsFile(tsFile);
END_TEST

void parse_exports_suite(Suite *suite) {
  TCase *tc_parse_exports = tcase_create("Parse exports");

  tcase_add_test(tc_parse_exports, parse_valid_export);
  tcase_add_test(tc_parse_exports, parse_invalid_export_with_unexpected_end_of_stream);
  tcase_add_test(tc_parse_exports, parse_invalid_export_with_newline);

  suite_add_tcase(suite, tc_parse_exports);
}