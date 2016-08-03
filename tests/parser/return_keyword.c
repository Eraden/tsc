#include "./return_keyword.h"

START_TEST(parse_empty_return)
  {
    char *content = "return;";
    TSFile tsFile = build_ts_file("memory.ts", content);
    validate_ts_file(tsFile, 1, TS_RETURN);

    TSParserToken returnToken = tsFile.tokens[0];

    ck_assert_int_eq(returnToken.childrenSize, 0);
    ck_assert_ptr_eq(returnToken.children, NULL);

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_return_number)
  {
    char *content = "return 1;";
    TSFile tsFile = build_ts_file("memory.ts", content);
    validate_ts_file(tsFile, 1, TS_RETURN);

    TSParserToken returnToken = tsFile.tokens[0];

    ck_assert_int_eq(returnToken.childrenSize, 1);
    ck_assert_ptr_ne(returnToken.children, NULL);

    TSParserToken child = returnToken.children[0];
    ck_assert(child.tokenType == TS_UNKNOWN);
    ck_assert_ptr_ne(child.data, NULL);
    ck_assert_wstr_eq(child.data, L"1");;

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_return_object)
  {
// // TODO: Decide JSON Object annotation is allowed?
//    char *content = "return {};";
//    TSFile tsFile = build_ts_file("memory.ts", content);
//    validate_ts_file(tsFile, 1, TS_RETURN);
//
//    TSParserToken returnToken = tsFile.tokens[0];
//
//    ck_assert_int_eq(returnToken.childrenSize, 1);
//    ck_assert_ptr_ne(returnToken.children, NULL);
//
//    TSParserToken child = returnToken.children[0];
//    ck_assert(child.tokenType == TS_UNKNOWN);
//    ck_assert_ptr_ne(child.data, NULL);
//    ck_assert_wstr_eq(child.data, L"{}");;
  }
END_TEST

START_TEST(parse_return_true)
  {
    char *content = "return true";
    TSFile tsFile = build_ts_file("memory.ts", content);
    validate_ts_file(tsFile, 1, TS_RETURN);

    TSParserToken returnToken = tsFile.tokens[0];

    ck_assert_int_eq(returnToken.childrenSize, 1);
    ck_assert_ptr_ne(returnToken.children, NULL);

    TSParserToken child = returnToken.children[0];
    ck_assert(child.tokenType == TS_UNKNOWN);
    ck_assert_ptr_ne(child.data, NULL);
    ck_assert_wstr_eq(child.data, L"true");;

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_return_false)
  {
    char *content = "return false";
    TSFile tsFile = build_ts_file("memory.ts", content);
    validate_ts_file(tsFile, 1, TS_RETURN);

    TSParserToken returnToken = tsFile.tokens[0];

    ck_assert_int_eq(returnToken.childrenSize, 1);
    ck_assert_ptr_ne(returnToken.children, NULL);

    TSParserToken child = returnToken.children[0];
    ck_assert(child.tokenType == TS_UNKNOWN);
    ck_assert_ptr_ne(child.data, NULL);
    ck_assert_wstr_eq(child.data, L"false");;

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_return_new_object)
  {
    char *content = "return new Object";
    TSFile tsFile = build_ts_file("memory.ts", content);
    validate_ts_file(tsFile, 1, TS_RETURN);

    TSParserToken returnToken = tsFile.tokens[0];

    ck_assert_int_eq(returnToken.childrenSize, 1);
    ck_assert_ptr_ne(returnToken.children, NULL);

    TSParserToken child = returnToken.children[0];
    ck_assert(child.tokenType == TS_NEW);
    ck_assert_ptr_ne(child.data, NULL);
    ck_assert_wstr_eq(child.data, L"Object");;

    TS_free_tsFile(tsFile);
  }
END_TEST

void parse_return_keyword_suite(Suite *suite) {
  TCase *tc_return_keyword = tcase_create("Return keyword");

  tcase_add_test(tc_return_keyword, parse_empty_return);
  tcase_add_test(tc_return_keyword, parse_return_number);
  tcase_add_test(tc_return_keyword, parse_return_object);
  tcase_add_test(tc_return_keyword, parse_return_true);
  tcase_add_test(tc_return_keyword, parse_return_false);
  tcase_add_test(tc_return_keyword, parse_return_new_object);

  suite_add_tcase(suite, tc_return_keyword);
}