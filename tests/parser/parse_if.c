#include "parse_if.h"

START_TEST(parse_valid_if_condition)
  TSFile *tsFile = TS_parse_file("./examples/if/valid.ts");

  ck_assert_int_eq(tsFile->tokensSize, 5);

  TSParserToken *token, *cond, *child;
  TSIfData *data;

  token = tsFile->tokens[0];
  data = token->ifData;
  ck_assert(token->tokenType == TS_IF);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_int_eq(data->conditionsSize, 1);
  cond = data->conditions[0];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"1");
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);

  token = tsFile->tokens[1];
  data = token->ifData;
  ck_assert(token->tokenType == TS_IF);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_int_eq(data->conditionsSize, 1);
  cond = data->conditions[0];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"2");
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);

  token = tsFile->tokens[2];
  data = token->data;
  ck_assert(token->tokenType == TS_IF);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_int_eq(data->conditionsSize, 5);
  cond = data->conditions[0];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"3");
  cond = data->conditions[1];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"==");
  cond = data->conditions[2];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"2");
  cond = data->conditions[3];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"&&");
  cond = data->conditions[4];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"true");
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);

  token = tsFile->tokens[3];
  data = token->data;
  ck_assert(token->tokenType == TS_IF);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_int_eq(data->conditionsSize, 1);
  cond = data->conditions[0];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"4");
  ck_assert_int_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  child = token->children[0];
  ck_assert(child->tokenType == TS_RETURN);
  ck_assert_ptr_eq(child->data, NULL);
  ck_assert_int_eq(child->childrenSize, 1);

  token = tsFile->tokens[4];
  data = token->data;
  ck_assert(token->tokenType == TS_IF);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_int_eq(data->conditionsSize, 1);
  cond = data->conditions[0];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"5");
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  child = token->children[0];
  TSLocalVariableData *varData = child->data;
  ck_assert(child->tokenType == TS_VAR);
  ck_assert_ptr_ne(varData, NULL);
  ck_assert_ptr_ne(varData->name, NULL);
  ck_assert_wstr_eq(varData->name, L"local");
  ck_assert_ptr_eq(varData->type, NULL);
  ck_assert_ptr_ne(varData->value, NULL);
  ck_assert_wstr_eq(varData->value, L"10");
  ck_assert_int_eq(child->childrenSize, 0);
  child = token->children[1];
  ck_assert(child->tokenType == TS_RETURN);
  ck_assert_ptr_eq(child->data, NULL);
  ck_assert_int_eq(child->childrenSize, 1);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_if_without_args)
  TS_parse_file("./examples/if/no_args");
END_TEST

START_TEST(parse_if_without_end_bracket)
  TS_parse_file("./examples/if/no_ending_bracket");
END_TEST

void parse_if_conditions_suite(Suite *suite) {
  TCase *tc_if_conditions = tcase_create("If Conditions");

  tcase_add_test(tc_if_conditions, parse_valid_if_condition);

  tcase_add_exit_test(tc_if_conditions, parse_if_without_args, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_if_conditions, parse_if_without_end_bracket, TS_PARSE_FAILURE_CODE);

  suite_add_tcase(suite, tc_if_conditions);
}