#include "./if_conditions.h"

START_TEST(parse_valid_else_condition)
  TSFile *tsFile = TS_parse_file("./examples/else/valid.ts");

  ck_assert_int_eq(tsFile->tokensSize, 4);

  TSParserToken *token, *child, *retValue;

  // 1
  token = tsFile->tokens[0];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  token = token->children[0];
  ck_assert(token->tokenType == TS_ELSE);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);

  // 2
  token = tsFile->tokens[1];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  token = token->children[0];
  ck_assert(token->tokenType == TS_ELSE);
  ck_assert_int_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);

  child = token->children[0];
  ck_assert(child->tokenType == TS_RETURN);
  ck_assert_int_eq(child->childrenSize, 1);
  ck_assert_ptr_ne(child->children, NULL);
  retValue = child->children[0];
  ck_assert(retValue->tokenType == TS_UNKNOWN);
  ck_assert_ptr_ne(retValue->name, NULL);
  ck_assert_wstr_eq(retValue->name, L"10");

  // 3
  token = tsFile->tokens[2];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  token = token->children[0];
  ck_assert(token->tokenType == TS_ELSE);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);

  // 4
  token = tsFile->tokens[3];
  ck_assert(token->tokenType == TS_IF);
  ck_assert_int_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  token = token->children[0];
  ck_assert(token->tokenType == TS_ELSE);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);

  TSParserToken *val = token->children[0];
  TSLocalVariableData *data = val->data;
  ck_assert(val->tokenType == TS_VAR);
  ck_assert_ptr_ne(val->data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"variable");
  ck_assert_ptr_eq(data->type, NULL);
  ck_assert_ptr_ne(data->value, NULL);
  ck_assert_wstr_eq(data->value, L"30");

  child = token->children[1];
  ck_assert(child->tokenType == TS_RETURN);
  ck_assert_int_eq(child->childrenSize, 1);
  ck_assert_ptr_ne(child->children, NULL);
  retValue = child->children[0];
  ck_assert(retValue->tokenType == TS_UNKNOWN);
  ck_assert_ptr_ne(retValue->data, NULL);
  ck_assert_wstr_eq(retValue->data, L"20");

  TS_free_tsFile(tsFile);
END_TEST

void parse_else_conditions_suite(Suite *suite) {
  TCase *tc_if_conditions = tcase_create("Else Conditions");

  tcase_add_test(tc_if_conditions, parse_valid_else_condition);

  suite_add_tcase(suite, tc_if_conditions);
}