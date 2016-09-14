#include "./parse_break.h"

START_TEST(parse_valid_break)
  TSFile *tsFile = TS_parse_file("./examples/break/valid.ts");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_VALID);
  TS_free_tsFile(tsFile);
END_TEST

START_TEST(break_unexpected_token)
  TSFile *tsFile = TS_parse_file("./examples/break/unexpected_token");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

void
parse_break_suite(Suite *suite) {
  TCase *tc_parse_switch = tcase_create("Parse switch");

  tcase_add_test(tc_parse_switch, parse_valid_break);
  tcase_add_test(tc_parse_switch, break_unexpected_token);

  suite_add_tcase(suite, tc_parse_switch);
}