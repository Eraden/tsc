#include "./parse_switch.h"

START_TEST(parse_valid_switch)
  TSFile *file = TS_parse_file("./examples/switch/valid.ts");

  ck_assert_ptr_ne(file, NULL);
  ck_assert(file->sanity == TS_FILE_VALID);

  TS_free_tsFile(file);
END_TEST

void parse_switch_suite(Suite *suite)
{
  TCase *tc_parse_switch = tcase_create("Parse switch");

  tcase_add_test(tc_parse_switch, parse_valid_switch);

  suite_add_tcase(suite, tc_parse_switch);
}