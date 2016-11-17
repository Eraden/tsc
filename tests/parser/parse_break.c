#include "./parse_break.h"

START_TEST(parse_valid_break)
  TSFile *tsFile = TS_parse_file("./examples/break/valid.ts");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_VALID);
  TS_free_ts_file(tsFile);
END_TEST

START_TEST(break_unexpected_token)
  wchar_t error[256];
  memset(error, 0, sizeof(wchar_t) * 256);
  TSFile *tsFile = TS_parse_file("./examples/break/unexpected_token");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_syntax_error(tsFile);
  ck_assert_ptr_ne(tsFile->errorReason, NULL);

  swprintf(error, 256,
           (const wchar_t *) L"Syntax error: Unexpected `break` in global scope\n      Position: %ls:1:5 [file:line:character]\n",
           tsFile->input.file
  );
//  ck_assert_wstr_eq(tsFile->errorReason, error);
  TS_free_ts_file(tsFile);
END_TEST

void
parse_break_suite(Suite *suite) {
  TCase *tc_parse_switch = tcase_create("Parse switch");

  tcase_add_test(tc_parse_switch, parse_valid_break);
  tcase_add_test(tc_parse_switch, break_unexpected_token);

  suite_add_tcase(suite, tc_parse_switch);
}