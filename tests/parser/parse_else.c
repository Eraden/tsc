#include "parse_if.h"

START_TEST(parse_valid_else_condition)
  TSFile *tsFile = TS_parse_file("./examples/else/valid.ts");

  ck_assert_int_eq(tsFile->tokensSize, 6);
  ck_assert_tsFile_valid(tsFile);

  TSParserToken *ifToken = NULL, *child = NULL, *scope = NULL, *val = NULL, *semicolon = NULL, *elseToken = NULL, *condition = NULL;

  // if (1); else;
  ifToken = tsFile->tokens[0];
  ck_assert_eq_ts_if(ifToken->tokenType);
  ck_assert_ulong_eq(ifToken->childrenSize, 3);
  ck_assert_ptr_ne(ifToken->children, NULL);
  condition = ifToken->children[0];
  ck_assert_eq_ts_condition(condition->tokenType);
  scope = ifToken->children[1];
  ck_assert_eq_ts_scope(scope->tokenType);
  elseToken = ifToken->children[2];
  ck_assert_eq_ts_else(elseToken->tokenType);

  // if (2); else const a = 10;
  ifToken = tsFile->tokens[1];
  ck_assert_eq_ts_if(ifToken->tokenType);
  ck_assert_int_eq(ifToken->childrenSize, 3);
  ck_assert_ptr_ne(ifToken->children, NULL);
  condition = ifToken->children[0];
  ck_assert_eq_ts_condition(condition->tokenType);
  scope = ifToken->children[1];
  ck_assert_eq_ts_scope(scope->tokenType);
  elseToken = ifToken->children[2];
  ck_assert_eq_ts_else(elseToken->tokenType);
  child = elseToken->children[0];
  ck_assert_eq_ts_const(child->tokenType);
  ck_assert_ptr_ne(child->name, NULL);
  ck_assert_wstr_eq(child->name, L"a");

  semicolon = tsFile->tokens[2];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // if (3); else { }
  ifToken = tsFile->tokens[3];
  ck_assert_eq_ts_if(ifToken->tokenType);
  ck_assert_ulong_eq(ifToken->childrenSize, 3);
  ck_assert_ptr_ne(ifToken->children, NULL);
  condition = ifToken->children[0];
  ck_assert_eq_ts_condition(condition->tokenType);
  scope = ifToken->children[1];
  ck_assert_eq_ts_scope(scope->tokenType);
  elseToken = ifToken->children[2];
  ck_assert_eq_ts_else(elseToken->tokenType);
  ck_assert_int_eq(elseToken->childrenSize, 1);
  ck_assert_ptr_ne(elseToken->children, NULL);
  scope = elseToken->children[0];
  ck_assert_eq_ts_scope(scope->tokenType);

  /* if (4); else { var variable = 30; const b = 20; } */
  ifToken = tsFile->tokens[4];
  ck_assert_eq_ts_if(ifToken->tokenType);
  ck_assert_int_eq(ifToken->childrenSize, 3);
  ck_assert_ptr_ne(ifToken->children, NULL);
  condition = ifToken->children[0];
  ck_assert_eq_ts_condition(condition->tokenType);
  scope = ifToken->children[1];
  ck_assert_eq_ts_scope(scope->tokenType);
  elseToken = ifToken->children[2];
  ck_assert_eq_ts_else(elseToken->tokenType);
  ck_assert_int_eq(elseToken->childrenSize, 1);
  ck_assert_ptr_ne(elseToken->children, NULL);
  scope = elseToken->children[0];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_int_eq(scope->childrenSize, 4);
  ck_assert_ptr_ne(scope->children, NULL);
  val = scope->children[0];
  ck_assert_eq_ts_var(val->tokenType);
  ck_assert_ptr_ne(val->data, NULL);
  ck_assert_ptr_ne(val->name, NULL);
  ck_assert_wstr_eq(val->name, L"variable");
  semicolon = scope->children[1];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);
  val = scope->children[2];
  ck_assert_eq_ts_const(val->tokenType);
  ck_assert_ptr_ne(val->name, NULL);
  ck_assert_wstr_eq(val->name, L"b");
  semicolon = scope->children[3];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);
  
  /* class Test { method() { if (1) return "true"; else return "false"; } } */

  TS_free_ts_file(tsFile);
END_TEST

void parse_else_conditions_suite(Suite *suite) {
  TCase *tc_if_conditions = tcase_create("Else Conditions");

  tcase_add_test(tc_if_conditions, parse_valid_else_condition);

  suite_add_tcase(suite, tc_if_conditions);
}