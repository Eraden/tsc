#include "./if_conditions.h"

START_TEST(parse_empty_if_condition)
  {
    const TSFile tsFile = build_ts_file("memory.ts", "if (1);");
    validate_ts_file(tsFile, 1, TS_IF);

    const TSParserToken token = tsFile.tokens[0];
    const TSIfData *data = token.data;

    ck_assert_ptr_eq(token.children, NULL);
    ck_assert_int_eq(token.childrenSize, 0);

    ck_assert_ptr_ne(data, NULL);
    ck_assert_int_eq(data->conditionsSize, 1);
  }
END_TEST

START_TEST(parse_empty_if_condition_with_brackets)
  {
    const TSFile tsFile = build_ts_file("memory.ts", "if (1) {}");
    validate_ts_file(tsFile, 1, TS_IF);

    const TSParserToken token = tsFile.tokens[0];
    const TSIfData *data = token.data;

    ck_assert_ptr_eq(token.children, NULL);
    ck_assert_int_eq(token.childrenSize, 0);

    ck_assert_ptr_ne(data, NULL);
    ck_assert_int_eq(data->conditionsSize, 1);
  }
END_TEST

START_TEST(parse_empty_if_condition_conditions)
  {
    const TSFile tsFile = build_ts_file("memory.ts", "if (1=2 && true) {}");
    validate_ts_file(tsFile, 1, TS_IF);

    const TSParserToken token = tsFile.tokens[0];
    const TSIfData *data = token.data;

    ck_assert_ptr_eq(token.children, NULL);
    ck_assert_int_eq(token.childrenSize, 0);

    ck_assert_ptr_ne(data, NULL);
    ck_assert_int_eq(data->conditionsSize, 6);

    const TSParserToken left = data->conditions[0];
    const TSParserToken eq = data->conditions[1];
    const TSParserToken right = data->conditions[2];
    const TSParserToken firstAmpersand = data->conditions[3];
    const TSParserToken secondAmpersand = data->conditions[4];
    const TSParserToken trueToken = data->conditions[5];

    ck_assert_ptr_ne(left.data, NULL);
    ck_assert_str_eq(left.data, "1");

    ck_assert_ptr_ne(eq.data, NULL);
    ck_assert_str_eq(eq.data, "=");

    ck_assert_ptr_ne(right.data, NULL);
    ck_assert_str_eq(right.data, "2");

    ck_assert_ptr_ne(firstAmpersand.data, NULL);
    ck_assert_str_eq(firstAmpersand.data, "&");

    ck_assert_ptr_ne(secondAmpersand.data, NULL);
    ck_assert_str_eq(secondAmpersand.data, "&");

    ck_assert_ptr_ne(trueToken.data, NULL);
    ck_assert_str_eq(trueToken.data, "true");
  }
END_TEST

START_TEST(parse_if_condition_with_body)
  {
    const TSFile tsFile = build_ts_file("memory.ts", "if (1) { return 1; }");
    validate_ts_file(tsFile, 1, TS_IF);

    const TSParserToken token = tsFile.tokens[0];
    const TSIfData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_int_eq(data->conditionsSize, 1);

    ck_assert_ptr_ne(token.children, NULL);
    ck_assert_int_eq(token.childrenSize, 1);

    const TSParserToken returnToken = token.children[0];
    ck_assert(returnToken.tokenType == TS_RETURN);
  }
END_TEST

START_TEST(parse_if_condition_without_brackets_with_body)
  {
    const TSFile tsFile = build_ts_file("memory.ts", "if (1) return 1;");
    validate_ts_file(tsFile, 1, TS_IF);

    const TSParserToken token = tsFile.tokens[0];
    const TSIfData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_int_eq(data->conditionsSize, 1);

    ck_assert_ptr_ne(token.children, NULL);
  ck_assert_int_eq(token.childrenSize, 1);

    const TSParserToken returnToken = token.children[0];
    ck_assert(returnToken.tokenType == TS_RETURN);
  }
END_TEST

void parse_if_conditions_suite(Suite *suite) {
  TCase *tc_if_conditions = tcase_create("If Conditions");

  tcase_add_test(tc_if_conditions, parse_empty_if_condition);
  tcase_add_test(tc_if_conditions, parse_empty_if_condition_with_brackets);
  tcase_add_test(tc_if_conditions, parse_empty_if_condition_conditions);
  tcase_add_test(tc_if_conditions, parse_if_condition_with_body);
  tcase_add_test(tc_if_conditions, parse_if_condition_without_brackets_with_body);

  suite_add_tcase(suite, tc_if_conditions);
}