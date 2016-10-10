#include "./parse_extends.h"

START_TEST(parse_valid_extends)
  TSFile *tsFile = TS_parse_file("./examples/extends/valid.ts");

  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_valid(tsFile);
  ck_assert_ulong_eq(tsFile->tokensSize, 3);

  TSParserToken *classToken = NULL;

  classToken = tsFile->tokens[0];
  ck_assert_eq_ts_class(classToken->tokenType);

  classToken = tsFile->tokens[1];
  ck_assert_eq_ts_class(classToken->tokenType);

  classToken = tsFile->tokens[2];
  ck_assert_eq_ts_class(classToken->tokenType);

  TS_free_tsFile(tsFile);
END_TEST

void parse_extends_suite(Suite *suite) {
  TCase *tc_extends = tcase_create("Variables");

  tcase_add_test(tc_extends, parse_valid_extends);
  suite_add_tcase(suite, tc_extends);
}
