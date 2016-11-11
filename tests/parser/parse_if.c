#include "parse_if.h"

START_TEST(parse_valid_if_condition)
  TSFile *tsFile = TS_parse_file("./examples/if/valid.ts");

  ck_assert_tsFile_valid(tsFile);
  ck_assert_int_eq(tsFile->tokensSize, 6);

  TSParserToken *token = NULL, *cond = NULL, *data = NULL, *scope = NULL, *constToken = NULL, *varToken = NULL, *semicolon = NULL;

  token = tsFile->tokens[0];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  cond = token->children[0];
  ck_assert_eq_ts_condition(cond->tokenType);
  ck_assert_ptr_ne(cond->children, NULL);
  data = cond->children[0];
  ck_assert_wstr_eq(data->content, L"1");
  cond = token->children[1];
  ck_assert_eq_ts_semicolon(cond->tokenType);

  token = tsFile->tokens[1];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  cond = token->children[0];
  ck_assert_eq_ts_condition(cond->tokenType);
  ck_assert_ptr_ne(cond->children, NULL);
  data = cond->children[0];
  ck_assert_wstr_eq(data->content, L"2");
  scope = token->children[1];
  ck_assert_eq_ts_scope(scope->tokenType);

  token = tsFile->tokens[2];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 6);
  ck_assert_ptr_ne(token->children, NULL);
  cond = token->children[0];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_ptr_ne(cond->children, NULL);
  data = cond->children[0];
  ck_assert_wstr_eq(data->content, L"3");
  cond = token->children[1];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_ptr_ne(cond->children, NULL);
  data = cond->children[0];
  TS_check_validate_borrow(data, TS_EQUAL);
  cond = token->children[2];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_ptr_ne(cond->children, NULL);
  data = cond->children[0];
  ck_assert_wstr_eq(data->content, L"2");
  cond = token->children[3];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_ptr_ne(cond->children, NULL);
  data = cond->children[0];
  TS_check_validate_borrow(data, TS_LOGICAL_AND);
  cond = token->children[4];
  ck_assert(cond->tokenType == TS_CONDITION);
  ck_assert_ptr_ne(cond->children, NULL);
  data = cond->children[0];
  ck_assert_wstr_eq(data->content, L"true");
  scope = token->children[5];
  ck_assert_eq_ts_scope(scope->tokenType);

  token = tsFile->tokens[3];
  ck_assert_eq_ts_if(token->tokenType);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  cond = token->children[0];
  ck_assert_eq_ts_condition(cond->tokenType);
  ck_assert_ptr_ne(cond->children, NULL);
  data = cond->children[0];
  ck_assert_wstr_eq(data->content, L"4");
  constToken = token->children[1];
  ck_assert_eq_ts_const(constToken->tokenType);
  ck_assert_ptr_ne(constToken->name, NULL);
  ck_assert_ptr_ne(constToken->children, NULL);
  ck_assert_wstr_eq(constToken->name, L"x");

  token = tsFile->tokens[4];
  ck_assert_eq_ts_semicolon(token->tokenType);

  token = tsFile->tokens[5];
  ck_assert_eq_ts_if(token->tokenType);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  cond = token->children[0];
  ck_assert_eq_ts_condition(cond->tokenType);
  ck_assert_ptr_ne(cond->children, NULL);
  data = cond->children[0];
  ck_assert_wstr_eq(data->content, L"5");
  scope = token->children[1];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_uint_eq(scope->childrenSize, 4);

  varToken = scope->children[0];
  ck_assert_eq_ts_var(varToken->tokenType);
  ck_assert_ptr_ne(varToken->name, NULL);
  ck_assert_wstr_eq(varToken->name, L"local");
  TS_check_validate_borrow(varToken->children[TS_VARIABLE_TYPE], ANY);
  ck_assert_wstr_eq(varToken->children[TS_VARIABLE_VALUE]->content, L"10");

  semicolon = scope->children[1];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  constToken = scope->children[2];
  ck_assert_eq_ts_const(constToken->tokenType);
  ck_assert_ptr_ne(constToken->name, NULL);
  ck_assert_wstr_eq(constToken->name, L"y");
  TS_check_validate_borrow(constToken->children[TS_VARIABLE_TYPE], ANY);

  semicolon = scope->children[3];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

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