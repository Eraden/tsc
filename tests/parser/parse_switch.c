#include "./parse_switch.h"

START_TEST(parse_valid_switch)
  TSFile *tsFile = TS_parse_file("./examples/switch/valid.ts");

  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_valid(tsFile);

  ck_assert_uint_eq(tsFile->tokensSize, 2);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

  TSParserToken *token = NULL, *child = NULL, *fnChild = NULL, *callArgs = NULL, *returnType = NULL,
      *scope = NULL, *conditions = NULL, *switchBody = NULL;

  token = tsFile->tokens[0];
  ck_assert_ptr_ne(token, NULL);
  ck_assert_eq_ts_switch(token->tokenType);
  ck_assert_uint_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  conditions = token->children[TS_SWITCH_CONDITIONS_INDEX];
  ck_assert_eq_ts_condition(conditions->tokenType);
  switchBody = token->children[TS_SWITCH_BODY_INDEX];
  ck_assert_ptr_ne(switchBody, NULL);
  ck_assert_eq_ts_switch_body(switchBody->tokenType);
  ck_assert_ptr_ne(switchBody->children, NULL);
  child = switchBody->children[0];
  ck_assert_ptr_ne(child, NULL);
  ck_assert_eq_ts_case(child->tokenType);
  child = switchBody->children[1];
  ck_assert_ptr_ne(child, NULL);
  ck_assert_eq_ts_default(child->tokenType);

  token = tsFile->tokens[1];
  ck_assert_ptr_ne(token, NULL);
  ck_assert_eq_ts_function(token->tokenType);
  ck_assert_ptr_ne(token->children, NULL);
  ck_assert_int_eq(token->childrenSize, 3);
  callArgs = token->children[0];
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);
  returnType = token->children[1];
  ck_assert_eq_ts_function_return_type(returnType->tokenType);
  scope = token->children[2];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_ptr_ne(scope->children, NULL);
  ck_assert_uint_eq(scope->childrenSize, 1);
  fnChild = scope->children[0];
  ck_assert_eq_ts_switch(fnChild->tokenType);
  ck_assert_ptr_ne(fnChild->children, NULL);
  ck_assert_int_eq(fnChild->childrenSize, 2);
  conditions = fnChild->children[TS_SWITCH_CONDITIONS_INDEX];
  ck_assert_eq_ts_condition(conditions->tokenType);
  switchBody = fnChild->children[TS_SWITCH_BODY_INDEX];
  ck_assert_ptr_ne(switchBody, NULL);
  ck_assert_eq_ts_switch_body(switchBody->tokenType);
  ck_assert_ptr_ne(switchBody->children, NULL);
  child = switchBody->children[0];
  ck_assert_ptr_ne(child, NULL);
  ck_assert_eq_ts_case(child->tokenType);
  child = switchBody->children[1];
  ck_assert_ptr_ne(child, NULL);
  ck_assert_eq_ts_default(child->tokenType);

  TS_free_ts_file(tsFile);
END_TEST

// SWITCH
// switch head
START_TEST(switch_head_unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/switch/head/unexpected_end_of_stream");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_syntax_error(tsFile);
  TS_free_ts_file(tsFile);
END_TEST

START_TEST(switch_head_unexpected_token)
  TSFile *tsFile = TS_parse_file("./examples/switch/head/unexpected_token");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_syntax_error(tsFile);
  TS_free_ts_file(tsFile);
END_TEST

// switch body
START_TEST(switch_body_unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/switch/body/unexpected_end_of_stream");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_syntax_error(tsFile);
  TS_free_ts_file(tsFile);
END_TEST

START_TEST(switch_body_unexpected_token)
  TSFile *tsFile = TS_parse_file("./examples/switch/body/unexpected_token");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_syntax_error(tsFile);
  TS_free_ts_file(tsFile);
END_TEST

// switch conditions
START_TEST(switch_conditions_unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/switch/conditions/unexpected_end_of_stream");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_syntax_error(tsFile);
  TS_free_ts_file(tsFile);
END_TEST

START_TEST(switch_conditions_unexpected_token)
  TSFile *tsFile = TS_parse_file("./examples/switch/conditions/unexpected_token");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_syntax_error(tsFile);
  TS_free_ts_file(tsFile);
END_TEST

// CASE
// case body
START_TEST(case_body_unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/case/body/unexpected_end_of_stream");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_syntax_error(tsFile);
  TS_free_ts_file(tsFile);
END_TEST

// case conditions
START_TEST(case_conditions_unexpected_end_of_stream)
  TSFile *tsFile = TS_parse_file("./examples/case/conditions/unexpected_end_of_stream");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_syntax_error(tsFile);
  TS_free_ts_file(tsFile);
END_TEST

START_TEST(case_conditions_unexpected_token)
  TSFile *tsFile = TS_parse_file("./examples/case/conditions/unexpected_token");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_syntax_error(tsFile);
  TS_free_ts_file(tsFile);
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