#include "./if_conditions.h"

START_TEST(parse_valid_if_condition)
  TSFile tsFile = TS_parse_file("./examples/if/valid.ts");

  ck_assert_int_eq(tsFile.tokensSize, 5);

  TSParserToken token, cond;
  TSIfData *data;

  token = tsFile.tokens[0];
  data = token.data;
  ck_assert(token.tokenType == TS_IF);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_int_eq(data->conditionsSize, 1);
  cond = data->conditions[0];
  ck_assert(cond.tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond.data, L"1");

  token = tsFile.tokens[1];
  data = token.data;
  ck_assert(token.tokenType == TS_IF);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_int_eq(data->conditionsSize, 1);
  cond = data->conditions[0];
  ck_assert(cond.tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond.data, L"2");

  token = tsFile.tokens[2];
  data = token.data;
  ck_assert(token.tokenType == TS_IF);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_int_eq(data->conditionsSize, 5);
  cond = data->conditions[0];
  ck_assert(cond.tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond.data, L"3");
  cond = data->conditions[1];
  ck_assert(cond.tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond.data, L"==");
  cond = data->conditions[2];
  ck_assert(cond.tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond.data, L"2");
  cond = data->conditions[3];
  ck_assert(cond.tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond.data, L"&&");
  cond = data->conditions[4];
  ck_assert(cond.tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond.data, L"true");

  token = tsFile.tokens[3];
  data = token.data;
  ck_assert(token.tokenType == TS_IF);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_int_eq(data->conditionsSize, 1);
  cond = data->conditions[0];
  ck_assert(cond.tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond.data, L"4");

  token = tsFile.tokens[4];
  data = token.data;
  ck_assert(token.tokenType == TS_IF);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_int_eq(data->conditionsSize, 1);
  cond = data->conditions[0];
  ck_assert(cond.tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond.data, L"5");

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_if_without_args)
  TS_parse_file("./examples/if/no_args");
END_TEST

START_TEST(parse_if_without_end_bracket)
  TS_parse_file("./examples/if/no_ending_bracket");
END_TEST

void parse_if_conditions_suite(Suite *suite) {
  TCase *tc_if_conditions = tcase_create("If Conditions");

  tcase_add_test(tc_if_conditions, parse_valid_if_condition);

  tcase_add_exit_test(tc_if_conditions, parse_if_without_args, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_if_conditions, parse_if_without_end_bracket, TS_PARSE_FAILURE_CODE);

  suite_add_tcase(suite, tc_if_conditions);
}