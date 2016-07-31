#include "./multiline_comment.h"

START_TEST(parse_empty_multiline_comment)
  {
    TSFile tsFile = build_ts_file("memory.ts", "/**/");
    validate_ts_file(tsFile, 1, TS_MULTILINE_COMMENT);

    TSParserToken token = tsFile.tokens[0];

    ck_assert_ptr_eq(token.data, NULL);

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_multiline_comment_with_spaces)
  {
    TSFile tsFile = build_ts_file("memory.ts", "/*  */");
    validate_ts_file(tsFile, 1, TS_MULTILINE_COMMENT);

    TSParserToken token = tsFile.tokens[0];

    ck_assert_ptr_ne(token.data, NULL);
    ck_assert_str_eq(token.data, "  ");

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_multiline_comment_without_spaces_and_with_word)
  {
    TSFile tsFile = build_ts_file("memory.ts", "/*hello*/");
    validate_ts_file(tsFile, 1, TS_MULTILINE_COMMENT);

    TSParserToken token = tsFile.tokens[0];

    ck_assert_ptr_ne(token.data, NULL);
    ck_assert_str_eq(token.data, "hello");

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_multiline_comment_with_spaces_and_with_word)
  {
    TSFile tsFile = build_ts_file("memory.ts", "/* hello  */");
    validate_ts_file(tsFile, 1, TS_MULTILINE_COMMENT);

    TSParserToken token = tsFile.tokens[0];

    ck_assert_ptr_ne(token.data, NULL);
    ck_assert_str_eq(token.data, " hello  ");

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_multiline_comment_with_spaces_and_with_word_and_multiple_lines)
  {
    TSFile tsFile = build_ts_file("memory.ts", "/* \nhello\n  */");
    validate_ts_file(tsFile, 1, TS_MULTILINE_COMMENT);

    TSParserToken token = tsFile.tokens[0];

    ck_assert_ptr_ne(token.data, NULL);
    ck_assert_str_eq(token.data, " \nhello\n  ");

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_miltiline_comment_with_no_ending)
  {
    build_ts_file("memory.ts", "/**");
  }
END_TEST

void parse_multiline_comment_suite(Suite *suite) {
  TCase *tc_multiline = tcase_create("Parse multiline comment");

  tcase_add_test(tc_multiline, parse_empty_multiline_comment);
  tcase_add_test(tc_multiline, parse_multiline_comment_with_spaces);
  tcase_add_test(tc_multiline, parse_multiline_comment_without_spaces_and_with_word);
  tcase_add_test(tc_multiline, parse_multiline_comment_with_spaces_and_with_word);
  tcase_add_test(tc_multiline, parse_multiline_comment_with_spaces_and_with_word_and_multiple_lines);

  TCase *tc_multiline_failure = tcase_create("Parse multiline comment failure");
  tcase_add_exit_test(tc_multiline_failure, parse_miltiline_comment_with_no_ending, TS_PARSE_FAILURE_CODE);

  suite_add_tcase(suite, tc_multiline);
  suite_add_tcase(suite, tc_multiline_failure);
}