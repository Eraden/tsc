#include "./parse_switch.h"

START_TEST(parse_valid_switch)
  TSFile *tsFile = TS_parse_file("./examples/switch/valid.ts");

  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_VALID);

  ck_assert_uint_eq(tsFile->tokensSize, 2);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

  TSParserToken *token, *child, *fnChild;

  token = tsFile->tokens[0];
  ck_assert_ptr_ne(token, NULL);
  ck_assert(token->tokenType == TS_SWITCH);
  ck_assert_uint_eq(token->childrenSize, 3);
  ck_assert_ptr_ne(token->children, NULL);
  child = token->children[0];
  ck_assert_ptr_ne(child, NULL);
  ck_assert(child->tokenType == TS_CONDITION);
  ck_assert_ptr_ne(child->name, NULL);
  ck_assert_wstr_eq(child->name, L"1");
  child = token->children[1];
  ck_assert_ptr_ne(child, NULL);
  ck_assert(child->tokenType == TS_CASE);
  child = token->children[2];
  ck_assert_ptr_ne(child, NULL);
  ck_assert(child->tokenType == TS_DEFAULT);

  token = tsFile->tokens[1];
  ck_assert_ptr_ne(token, NULL);
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_ptr_ne(token->children, NULL);
  ck_assert_int_eq(token->childrenSize, 1);
  fnChild = token->children[0];
  ck_assert_ptr_ne(fnChild->children, NULL);
  ck_assert_int_eq(fnChild->childrenSize, 3);
  child = fnChild->children[0];
  ck_assert_ptr_ne(child, NULL);
  ck_assert(child->tokenType == TS_CONDITION);
  ck_assert_ptr_ne(child->name, NULL);
  ck_assert_wstr_eq(child->name, L"1");
  child = fnChild->children[1];
  ck_assert_ptr_ne(child, NULL);
  ck_assert(child->tokenType == TS_CASE);
  child = fnChild->children[2];
  ck_assert_ptr_ne(child, NULL);
  ck_assert(child->tokenType == TS_DEFAULT);

  TS_free_tsFile(tsFile);
END_TEST

void parse_switch_suite(Suite *suite)
{
  TCase *tc_parse_switch = tcase_create("Parse switch");

  tcase_add_test(tc_parse_switch, parse_valid_switch);

  suite_add_tcase(suite, tc_parse_switch);
}