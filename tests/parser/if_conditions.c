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

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
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
    ck_assert_wstr_eq(left.data, L"1");;

    ck_assert_ptr_ne(eq.data, NULL);
    ck_assert_wstr_eq(eq.data, L"=");;

    ck_assert_ptr_ne(right.data, NULL);
    ck_assert_wstr_eq(right.data, L"2");;

    ck_assert_ptr_ne(firstAmpersand.data, NULL);
    ck_assert_wstr_eq(firstAmpersand.data, L"&");;

    ck_assert_ptr_ne(secondAmpersand.data, NULL);
    ck_assert_wstr_eq(secondAmpersand.data, L"&");;

    ck_assert_ptr_ne(trueToken.data, NULL);
    ck_assert_wstr_eq(trueToken.data, L"true");;

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
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