#include "parse_if.h"

START_TEST(parse_valid_if_condition)
  TSFile *tsFile = TS_parse_file("./examples/if/valid.ts");

  ck_assert_tsFile_valid(tsFile);
  ck_assert_int_eq(tsFile->tokensSize, 5);

  TSParserToken *token, *cond, *child, *scope;

  token = tsFile->tokens[0];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  cond = token->children[0];
  ck_assert_eq_ts_condition(cond->tokenType);
  ck_assert_wstr_eq(cond->data, L"1");
  cond = token->children[1];
  ck_assert_eq_ts_unknown(cond->tokenType);
  ck_assert_wstr_eq(cond->data, L";");

  token = tsFile->tokens[1];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  cond = token->children[0];
  ck_assert_eq_ts_condition(cond->tokenType);
  ck_assert_wstr_eq(cond->data, L"2");
  scope = token->children[1];
  ck_assert_eq_ts_scope(scope->tokenType);

  token = tsFile->tokens[2];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 6);
  ck_assert_ptr_ne(token->children, NULL);
  cond = token->children[0];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"3");
  cond = token->children[1];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"==");
  cond = token->children[2];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"2");
  cond = token->children[3];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"&&");
  cond = token->children[4];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"true");
  scope = token->children[5];
  ck_assert_eq_ts_scope(scope->tokenType);

  token = tsFile->tokens[3];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  cond = token->children[0];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"4");
  child = token->children[1];
  ck_assert(child->tokenType == TS_RETURN);
  ck_assert_ptr_eq(child->data, NULL);
  ck_assert_int_eq(child->childrenSize, 1);

  token = tsFile->tokens[4];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  cond = token->children[0];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_wstr_eq(cond->data, L"5");
  scope = token->children[1];
  ck_assert_eq_ts_scope(scope->tokenType);
  child = scope->children[0];
  TSLocalVariableData *varData = child->data;
  ck_assert(child->tokenType == TS_VAR);
  ck_assert_ptr_ne(varData, NULL);
  ck_assert_ptr_ne(varData->name, NULL);
  ck_assert_wstr_eq(varData->name, L"local");
  ck_assert_ptr_eq(varData->type, NULL);
  ck_assert_ptr_ne(varData->value, NULL);
  ck_assert_wstr_eq(varData->value, L"10");
  ck_assert_int_eq(child->childrenSize, 0);
  child = scope->children[1];
  ck_assert_eq_ts_return(child->tokenType);
  ck_assert_ptr_eq(child->data, NULL);
  ck_assert_int_eq(child->childrenSize, 1);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_if_without_args)
  TSFile *tsFile = TS_parse_file("./examples/if/no_args");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_if_without_end_bracket)
  TSFile *tsFile = TS_parse_file("./examples/if/no_ending_bracket");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert(tsFile->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_tsFile(tsFile);
END_TEST

void parse_if_conditions_suite(Suite *suite) {
  TCase *tc_if_conditions = tcase_create("If Conditions");

  tcase_add_test(tc_if_conditions, parse_valid_if_condition);

  tcase_add_test(tc_if_conditions, parse_if_without_args);
  tcase_add_test(tc_if_conditions, parse_if_without_end_bracket);

  suite_add_tcase(suite, tc_if_conditions);
}