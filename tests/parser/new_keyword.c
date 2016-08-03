#include "./new_keyword.h"

START_TEST(parse_new_keyword_with_end)
  {
    TSFile tsFile = build_ts_file("memory.ts", "new Object;");
    validate_ts_file(tsFile, 1, TS_NEW);

    TSParserToken token = tsFile.tokens[0];
    ck_assert_ptr_ne(token.data, NULL);
    ck_assert_wstr_eq(token.data, L"Object");;

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_new_keyword_with_new_line)
  {
    TSFile tsFile = build_ts_file("memory.ts", "new Object\n");
    validate_ts_file(tsFile, 1, TS_NEW);

    TSParserToken token = tsFile.tokens[0];
    ck_assert_ptr_ne(token.data, NULL);
    ck_assert_wstr_eq(token.data, L"Object");;

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_new_keyword_without_end)
  {
    TSFile tsFile = build_ts_file("memory.ts", "new Object");
    validate_ts_file(tsFile, 1, TS_NEW);

    TSParserToken token = tsFile.tokens[0];
    ck_assert_ptr_ne(token.data, NULL);
    ck_assert_wstr_eq(token.data, L"Object");;

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_new_keyword_with_brackets)
  {
    TSFile tsFile = build_ts_file("memory.ts", "new Object()");
    validate_ts_file(tsFile, 1, TS_NEW);

    TSParserToken token = tsFile.tokens[0];
    ck_assert_ptr_ne(token.data, NULL);
    ck_assert_wstr_eq(token.data, L"Object()");;

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_invalid_new_keyword)
  {
    build_ts_file("memory.ts", "new");
  }
END_TEST

START_TEST(parse_invalid_new_keyword_with_new_line)
  {
    build_ts_file("memory.ts", "new\n");
  }
END_TEST

START_TEST(parse_invalid_new_keyword_with_end)
  {
    build_ts_file("memory.ts", "new;");
  }
END_TEST

void parse_new_suite(Suite *suite) {
  TCase *tc_new_keyword = tcase_create("Parse new keyword");

  tcase_add_test(tc_new_keyword, parse_new_keyword_with_end);
  tcase_add_test(tc_new_keyword, parse_new_keyword_with_new_line);
  tcase_add_test(tc_new_keyword, parse_new_keyword_without_end);
  tcase_add_test(tc_new_keyword, parse_new_keyword_with_brackets);

  tcase_add_exit_test(tc_new_keyword, parse_invalid_new_keyword, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_new_keyword, parse_invalid_new_keyword_with_new_line, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_new_keyword, parse_invalid_new_keyword_with_end, TS_PARSE_FAILURE_CODE);

  suite_add_tcase(suite, tc_new_keyword);
}