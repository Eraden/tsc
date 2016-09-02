#include "./parse_exports.h"

START_TEST(parse_valid_export)
  TSFile *tsFile = TS_parse_file("./examples/export/valid.ts");
  ck_assert_uint_eq(tsFile->tokensSize, 4);

  TSParserToken *token, *exported;

  token = tsFile->tokens[0];
  ck_assert(token->tokenType == TS_EXPORT);
  ck_assert_uint_eq(token->childrenSize, 1);
  exported = token->children[0];
  ck_assert(exported->tokenType == TS_FUNCTION);

  token = tsFile->tokens[1];
  ck_assert(token->tokenType == TS_EXPORT);
  ck_assert_uint_eq(token->childrenSize, 1);
  exported = token->children[0];
  ck_assert(exported->tokenType == TS_VAR);

  token = tsFile->tokens[2];
  ck_assert(token->tokenType == TS_EXPORT);
  ck_assert_uint_eq(token->childrenSize, 1);
  exported = token->children[0];
  ck_assert(exported->tokenType == TS_CONST);

  token = tsFile->tokens[3];
  ck_assert(token->tokenType == TS_EXPORT);
  ck_assert_uint_eq(token->childrenSize, 1);
  exported = token->children[0];
  ck_assert(exported->tokenType == TS_CLASS);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_invalid_export_with_newline)
  TS_parse_file("./examples/export/unexpected_new_line");
END_TEST

START_TEST(parse_invalid_export_with_unexpected_end_of_stream)
  TS_parse_file("./examples/export/unexpected_end_of_stream");
END_TEST

void parse_exports_suite(Suite *suite) {
  TCase *tc_parse_exports = tcase_create("Parse exports");

  tcase_add_test(tc_parse_exports, parse_valid_export);
  tcase_add_exit_test(tc_parse_exports, parse_invalid_export_with_unexpected_end_of_stream, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_parse_exports, parse_invalid_export_with_newline, TS_PARSE_FAILURE_CODE);

  suite_add_tcase(suite, tc_parse_exports);
}