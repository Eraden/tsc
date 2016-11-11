#include "parse_if.h"

START_TEST(parse_valid_else_condition)
  TSFile *tsFile = TS_parse_file("./examples/else/valid.ts");

  ck_assert_int_eq(tsFile->tokensSize, 5);
  ck_assert_tsFile_valid(tsFile);

  TSParserToken *token = NULL, *child = NULL, *scope = NULL, *val = NULL, *semicolon = NULL;

  // 1
  token = tsFile->tokens[0];
  ck_assert_eq_ts_if(token->tokenType);
  ck_assert_ulong_eq(token->childrenSize, 3);
  ck_assert_ptr_ne(token->children, NULL);
  child = token->children[0];
  ck_assert_eq_ts_condition(child->tokenType);
  child = token->children[1];
  ck_assert_eq_ts_semicolon(child->tokenType);
  child = token->children[2];
  ck_assert_eq_ts_else(child->tokenType);

  // 2
  token = tsFile->tokens[1];
  ck_assert_eq_ts_if(token->tokenType);
  ck_assert_int_eq(token->childrenSize, 3);
  ck_assert_ptr_ne(token->children, NULL);
  child = token->children[0];
  ck_assert_eq_ts_condition(child->tokenType);
  child = token->children[1];
  ck_assert_eq_ts_semicolon(child->tokenType);
  child = token->children[2];
  ck_assert_eq_ts_else(child->tokenType);

  child = child->children[0];
  ck_assert_eq_ts_const(child->tokenType);
  ck_assert_ptr_ne(child->name, NULL);
  ck_assert_wstr_eq(child->name, L"a");

  semicolon = tsFile->tokens[2];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // 3
  token = tsFile->tokens[3];
  ck_assert_eq_ts_if(token->tokenType);
  ck_assert_ulong_eq(token->childrenSize, 3);
  ck_assert_ptr_ne(token->children, NULL);
  child = token->children[0];
  ck_assert_eq_ts_condition(child->tokenType);
  child = token->children[1];
  ck_assert_eq_ts_semicolon(child->tokenType);
  child = token->children[2];
  ck_assert_eq_ts_else(child->tokenType);
  ck_assert_int_eq(child->childrenSize, 1);
  ck_assert_ptr_ne(child->children, NULL);
  scope = child->children[0];
  ck_assert_eq_ts_scope(scope->tokenType);

  // 4
  token = tsFile->tokens[4];
  ck_assert_eq_ts_if(token->tokenType);
  ck_assert_int_eq(token->childrenSize, 3);
  ck_assert_ptr_ne(token->children, NULL);
  child = token->children[0];
  ck_assert_eq_ts_condition(child->tokenType);
  child = token->children[1];
  ck_assert_eq_ts_semicolon(child->tokenType);
  child = token->children[2];
  ck_assert_eq_ts_else(child->tokenType);
  ck_assert_int_eq(child->childrenSize, 1);
  ck_assert_ptr_ne(child->children, NULL);

  scope = child->children[0];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_int_eq(scope->childrenSize, 4);
  ck_assert_ptr_ne(scope->children, NULL);

  val = scope->children[0];
  ck_assert_eq_ts_var(val->tokenType);
  ck_assert_ptr_ne(val->data, NULL);
  ck_assert_ptr_ne(val->name, NULL);
  ck_assert_wstr_eq(val->name, L"variable");
//  ck_assert_ptr_ne(data->value, NULL);
//  ck_assert_wstr_eq(data->value, L"30");

  semicolon = scope->children[1];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  val = scope->children[2];
  ck_assert_eq_ts_const(val->tokenType);
  ck_assert_ptr_ne(val->name, NULL);
  ck_assert_wstr_eq(val->name, L"b");

  semicolon = scope->children[3];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  TS_free_tsFile(tsFile);
END_TEST

void parse_else_conditions_suite(Suite *suite) {
  TCase *tc_if_conditions = tcase_create("Else Conditions");

  tcase_add_test(tc_if_conditions, parse_valid_else_condition);

  suite_add_tcase(suite, tc_if_conditions);
}