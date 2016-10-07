#include "./parse_exports.h"

START_TEST(parse_valid_export)
  TSFile *tsFile = TS_parse_file("./examples/export/valid.ts");
  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 4);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

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