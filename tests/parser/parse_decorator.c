#include "parse_decorator.h"

START_TEST(parse_valid_decorators)
  TSFile *tsFile = TS_parse_file("./examples/decorator/valid.ts");
  TS_free_tsFile(tsFile);
END_TEST

START_TEST(unexpected_end_of_stream)
  TS_parse_file("./examples/decorator/unexpected_end_of_stream");
END_TEST

START_TEST(unexpected_keyword)
  TS_parse_file("./examples/decorator/unexpected_keyword");
END_TEST

START_TEST(invalid_character)
  TS_parse_file("./examples/decorator/invalid_character");
END_TEST

void parse_decorator_suite(Suite *suite) {
  TCase *tc_decorator = tcase_create("Functions");

  tcase_add_test(tc_decorator, parse_valid_decorators);
  tcase_add_exit_test(tc_decorator, unexpected_end_of_stream, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_decorator, unexpected_keyword, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_decorator, invalid_character, TS_PARSE_FAILURE_CODE);

  suite_add_tcase(suite, tc_decorator);
}