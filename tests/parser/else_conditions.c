#include "./if_conditions.h"

START_TEST(parse_empty_else_condition)
  {
    const TSFile tsFile = build_ts_file("memory.ts", "else;");
    validate_ts_file(tsFile, 1, TS_ELSE);

    const TSParserToken token = tsFile.tokens[0];
    const void *data = token.data;

    ck_assert_ptr_eq(token.children, NULL);
    ck_assert_int_eq(token.childrenSize, 0);

    ck_assert_ptr_eq(data, NULL);

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_empty_else_condition_with_brackets)
  {
    const TSFile tsFile = build_ts_file("memory.ts", "else {}");
    validate_ts_file(tsFile, 1, TS_ELSE);

    const TSParserToken token = tsFile.tokens[0];
    const TSIfData *data = token.data;

    ck_assert_ptr_eq(token.children, NULL);
    ck_assert_int_eq(token.childrenSize, 0);

    ck_assert_ptr_eq(data, NULL);

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_empty_else_condition_with_children)
  {
    const TSFile tsFile = build_ts_file("memory.ts", "else return 10;");
    validate_ts_file(tsFile, 1, TS_ELSE);

    const TSParserToken token = tsFile.tokens[0];
    const TSIfData *data = token.data;

    ck_assert_ptr_ne(token.children, NULL);
    ck_assert_int_eq(token.childrenSize, 1);
    ck_assert(token.children[0].tokenType == TS_RETURN);

    ck_assert_ptr_eq(data, NULL);

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_empty_else_condition_with_children_and_brackets)
  {
    const TSFile tsFile = build_ts_file("memory.ts", "else { return 10; }");
    validate_ts_file(tsFile, 1, TS_ELSE);

    const TSParserToken token = tsFile.tokens[0];
    const TSIfData *data = token.data;

    ck_assert_ptr_ne(token.children, NULL);
    ck_assert_int_eq(token.childrenSize, 1);
    ck_assert(token.children[0].tokenType == TS_RETURN);

    ck_assert_ptr_eq(data, NULL);

    TS_free_tsFile(tsFile);
  }
END_TEST

void parse_else_conditions_suite(Suite *suite) {
  TCase *tc_if_conditions = tcase_create("Else Conditions");

  tcase_add_test(tc_if_conditions, parse_empty_else_condition);
  tcase_add_test(tc_if_conditions, parse_empty_else_condition_with_brackets);
  tcase_add_test(tc_if_conditions, parse_empty_else_condition_with_children);
  tcase_add_test(tc_if_conditions, parse_empty_else_condition_with_children_and_brackets);

  suite_add_tcase(suite, tc_if_conditions);
}