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

// SWITCH
// switch head
START_TEST(switch_head_unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/switch/head/unexpected_end_of_stream");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

START_TEST(switch_head_unexpected_token)
  TSFile *tsFile = TS_parse_file("./examples/switch/head/unexpected_token");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

// switch body
START_TEST(switch_body_unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/switch/body/unexpected_end_of_stream");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

START_TEST(switch_body_unexpected_token)
  TSFile *tsFile = TS_parse_file("./examples/switch/body/unexpected_token");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

// switch conditions
START_TEST(switch_conditions_unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/switch/conditions/unexpected_end_of_stream");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

START_TEST(switch_conditions_unexpected_token)
  TSFile *tsFile = TS_parse_file("./examples/switch/conditions/unexpected_token");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

// CASE
// case body
START_TEST(case_body_unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/case/body/unexpected_end_of_stream");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

// case conditions
START_TEST(case_conditions_unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/case/conditions/unexpected_end_of_stream");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

START_TEST(case_conditions_unexpected_token)
  TSFile *tsFile = TS_parse_file("./examples/case/conditions/unexpected_token");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

void parse_switch_suite(Suite *suite)
{
  TCase *tc_parse_switch = tcase_create("Parse switch");

  tcase_add_test(tc_parse_switch, parse_valid_switch);

  // switch
  // head
  tcase_add_test(tc_parse_switch, switch_head_unexpected_end_of_stream);
  tcase_add_test(tc_parse_switch, switch_head_unexpected_token);
  // body
  tcase_add_test(tc_parse_switch, switch_body_unexpected_end_of_stream);
  tcase_add_test(tc_parse_switch, switch_body_unexpected_token);
  // conditions
  tcase_add_test(tc_parse_switch, switch_conditions_unexpected_end_of_stream);
  tcase_add_test(tc_parse_switch, switch_conditions_unexpected_token);

  // case
  // body
  tcase_add_test(tc_parse_switch, case_body_unexpected_end_of_stream);
  // conditions
  tcase_add_test(tc_parse_switch, case_conditions_unexpected_end_of_stream);
  tcase_add_test(tc_parse_switch, case_conditions_unexpected_token);

  suite_add_tcase(suite, tc_parse_switch);
}