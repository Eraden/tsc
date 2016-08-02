#include "./parse_exports.h"

START_TEST(parse_export)
  const char *content = "export class A {}";
  TSFile tsFile = build_ts_file("memory.ts", content);
  validate_ts_file(tsFile, 1, TS_EXPORT);

  TSParserToken token = tsFile.tokens[0];

  ck_assert_int_eq(token.childrenSize, 1);

  TSParserToken classToken = token.children[0];

  ck_assert(classToken.tokenType == TS_CLASS);
  TSClassData *data = classToken.data;

  ck_assert_ptr_ne(data, NULL);
  ck_assert_str_eq(data->name, "A");

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_invalid_export_with_newline)
  build_ts_file("memory.ts", "export\n");
END_TEST

START_TEST(parse_invalid_export_with_unexpected_end_of_stream)
  build_ts_file("memory.ts", "export ");
END_TEST

void parse_exports_suite(Suite *suite) {
  TCase *tc_parse_exports = tcase_create("Parse exports");

  tcase_add_test(tc_parse_exports, parse_export);
  tcase_add_exit_test(tc_parse_exports, parse_invalid_export_with_unexpected_end_of_stream, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_parse_exports, parse_invalid_export_with_newline, TS_PARSE_FAILURE_CODE);

  suite_add_tcase(suite, tc_parse_exports);
}