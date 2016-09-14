#include "parse_decorator.h"

START_TEST(parse_valid_decorators)
  TSFile *tsFile = TS_parse_file("./examples/decorator/valid.ts");
  TS_free_tsFile(tsFile);
END_TEST

START_TEST(unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/decorator/unexpected_end_of_stream");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

START_TEST(unexpected_keyword)
  TSFile *tsFile = TS_parse_file("./examples/decorator/unexpected_keyword");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

START_TEST(invalid_character)
  TSFile *tsFile = TS_parse_file("./examples/decorator/invalid_character");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

void parse_decorator_suite(Suite *suite) {
  TCase *tc_decorator = tcase_create("Functions");

  tcase_add_test(tc_decorator, parse_valid_decorators);
  tcase_add_test(tc_decorator, unexpected_end_of_stream);
  tcase_add_test(tc_decorator, unexpected_keyword);
  tcase_add_test(tc_decorator, invalid_character);

  suite_add_tcase(suite, tc_decorator);
}