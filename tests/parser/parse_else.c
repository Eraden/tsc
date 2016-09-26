#include "parse_if.h"

START_TEST(parse_valid_else_condition)
  TSFile *tsFile = TS_parse_file("./examples/else/valid.ts");

  ck_assert_int_eq(tsFile->tokensSize, 4);
  ck_assert_tsFile_valid(tsFile);

  TSParserToken *token, *child, *retValue;

  // 1
  token = tsFile->tokens[0];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_ulong_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  child = token->children[0];
  ck_assert_eq_ts_condition(child->tokenType);
  child = token->children[1];
  ck_assert_eq_ts_else(child->tokenType);

  // 2
  token = tsFile->tokens[1];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  child = token->children[0];
  ck_assert_eq_ts_condition(child->tokenType);
  child = token->children[1];
  ck_assert_eq_ts_else(child->tokenType);

  child = child->children[0];
  ck_assert_eq_ts_return(child->tokenType);
  ck_assert_int_eq(child->childrenSize, 1);
  ck_assert_ptr_ne(child->children, NULL);
  retValue = child->children[0];
  ck_assert_eq_ts_unknown(retValue->tokenType);
  ck_assert_ptr_ne(retValue->name, NULL);
  ck_assert_wstr_eq(retValue->name, L"10");

  // 3
  token = tsFile->tokens[2];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_ulong_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  child = token->children[0];
  ck_assert_eq_ts_condition(child->tokenType);
  child = token->children[1];
  ck_assert_eq_ts_else(child->tokenType);
  ck_assert_int_eq(child->childrenSize, 0);
  ck_assert_ptr_eq(child->children, NULL);

  // 4
  token = tsFile->tokens[3];
  ck_assert_eq_ts_if(token->tokenType);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  child = token->children[0];
  ck_assert_eq_ts_condition(child->tokenType);
  child = token->children[1];
  ck_assert_eq_ts_else(child->tokenType);
  ck_assert_int_eq(child->childrenSize, 2);
  ck_assert_ptr_ne(child->children, NULL);

  TSParserToken *val = child->children[0];
  TSLocalVariableData *data = val->variableData;
  ck_assert_eq_ts_var(val->tokenType);
  ck_assert_ptr_ne(val->data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"variable");
  ck_assert_ptr_eq(data->type, NULL);
  ck_assert_ptr_ne(data->value, NULL);
  ck_assert_wstr_eq(data->value, L"30");

  TSParserToken *ret = child->children[1];
  ck_assert_eq_ts_return(ret->tokenType);
  ck_assert_ulong_eq(ret->childrenSize, 1);
  ck_assert_ptr_ne(ret->children, NULL);
  TSParserToken *retVal = ret->children[0];
  ck_assert_eq_ts_unknown(retVal->tokenType);
  ck_assert_ptr_ne(retVal->content, NULL);
  ck_assert_wstr_eq(retVal->content, L"20");

  TS_free_tsFile(tsFile);
END_TEST

void parse_else_conditions_suite(Suite *suite) {
  TCase *tc_if_conditions = tcase_create("Else Conditions");

  tcase_add_test(tc_if_conditions, parse_valid_else_condition);

  suite_add_tcase(suite, tc_if_conditions);
}