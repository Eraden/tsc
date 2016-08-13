#include "./inline_comment.h"

START_TEST(parse_valid_inline_comment)
  TSFile tsFile = TS_parse_file("./examples/inline_comment/valid.ts");
  
  ck_assert_uint_eq(tsFile.tokensSize, 4);
  
  TSParserToken token;
  
  token = tsFile.tokens[0];
  ck_assert(token.tokenType, TS_INLINE_COMMENT);
  ck_assert_uint_eq(token.childrenSize, 0);
  ck_assert_ptr_eq(token.children, NULL);
  ck_assert_ptr_ne(token.data, NULL);
  ck_assert_wstr_eq(token.data, L" Filled\n");
  
  token = tsFile.tokens[1];
  ck_assert(token.tokenType, TS_INLINE_COMMENT);
  ck_assert_uint_eq(token.childrenSize, 0);
  ck_assert_ptr_eq(token.children, NULL);
  ck_assert_ptr_ne(token.data, NULL);
  ck_assert_wstr_eq(token.data, L"  \n");
  
  token = tsFile.tokens[2];
  ck_assert(token.tokenType, TS_INLINE_COMMENT);
  ck_assert_uint_eq(token.childrenSize, 0);
  ck_assert_ptr_eq(token.children, NULL);
  ck_assert_ptr_ne(token.data, NULL);
  ck_assert_wstr_eq(token.data, L"\n");
  
  token = tsFile.tokens[3];
  ck_assert(token.tokenType, TS_INLINE_COMMENT);
  ck_assert_uint_eq(token.childrenSize, 0);
  ck_assert_ptr_eq(token.children, NULL);
  ck_assert_ptr_eq(token.data, NULL);
  
  TS_free_tsFile(tsFile);
END_TEST

void parse_inline_comment_suite(Suite *suite) {
  TCase *test_case = tcase_create("Parse Inline comment");

  tcase_add_test(test_case, parse_valid_inline_comment);

  suite_add_tcase(suite, test_case);
}
