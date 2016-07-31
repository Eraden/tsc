#include "./inline_comment.h"

START_TEST(parse_empty_inline_comment)
  {
    TSFile tsFile = build_ts_file("memory.ts", "//");
    validate_ts_file(tsFile, 1, TS_INLINE_COMMENT);

    TSParserToken token;
    token = tsFile.tokens[0];
    ck_assert_ptr_eq(token.data, NULL);

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_inline_comment_with_spaces)
  {
    TSFile tsFile = build_ts_file("memory.ts", "//  ");
    validate_ts_file(tsFile, 1, TS_INLINE_COMMENT);

    TSParserToken token;
    token = tsFile.tokens[0];
    ck_assert_ptr_ne(token.data, NULL);
    ck_assert_str_eq(token.data, "  ");

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_inline_comment_without_spaces_and_with_word)
  {
    TSFile tsFile = build_ts_file("memory.ts", "//hello");
    validate_ts_file(tsFile, 1, TS_INLINE_COMMENT);

    TSParserToken token;
    token = tsFile.tokens[0];
    ck_assert_ptr_ne(token.data, NULL);
    ck_assert_str_eq(token.data, "hello");

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_inline_comment_with_spaces_and_with_word)
  {
    TSFile tsFile = build_ts_file("memory.ts", "// hello ");
    validate_ts_file(tsFile, 1, TS_INLINE_COMMENT);

    TSParserToken token;
    token = tsFile.tokens[0];
    ck_assert_ptr_ne(token.data, NULL);
    ck_assert_str_eq(token.data, " hello ");

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_inline_comment_with_space_and_with_word_and_newline)
  {
    TSFile tsFile = build_ts_file("memory.ts", "// hello\n");
    validate_ts_file(tsFile, 1, TS_INLINE_COMMENT);

    TSParserToken token;
    token = tsFile.tokens[0];
    ck_assert_ptr_ne(token.data, NULL);
    ck_assert_str_eq(token.data, " hello");

    TS_free_tsFile(tsFile);
  }
END_TEST

void parse_inline_comment_suite(Suite *suite) {
  TCase *tc_multiline = tcase_create("Parse Inline comment");

  tcase_add_test(tc_multiline, parse_empty_inline_comment);
  tcase_add_test(tc_multiline, parse_inline_comment_with_spaces);
  tcase_add_test(tc_multiline, parse_inline_comment_without_spaces_and_with_word);
  tcase_add_test(tc_multiline, parse_inline_comment_with_spaces_and_with_word);
  tcase_add_test(tc_multiline, parse_inline_comment_with_space_and_with_word_and_newline);

  suite_add_tcase(suite, tc_multiline);
}