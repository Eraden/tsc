#include "./parse_spread.h"

START_TEST(parse_valid_spread)
END_TEST

void parse_spread_suite(Suite *suite) {
  TCase *tc_super = tcase_create("spread");

  tcase_add_test(tc_super, parse_valid_spread);

  suite_add_tcase(suite, tc_super);
}
