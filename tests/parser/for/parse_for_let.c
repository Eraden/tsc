#include "./parse_for_let.h"

START_TEST(parse_valid_for_let)
  TSFile *tsFile = TS_parse_file("./examples/for/let/valid.ts");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_valid(tsFile);
  ck_assert_ptr_ne(tsFile->tokens, NULL);
  ck_assert_uint_eq(tsFile->tokensSize, 3);

  TSParserToken *token, *head, *body, *child, *variablesSection, *conditionSection, *changeSection;

  token = tsFile->tokens[0];
  ck_assert_ptr_ne(token, NULL);
  ck_assert_eq_ts_for(token->tokenType);
  ck_assert_ulong_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  head = token->children[0];
  ck_assert_eq_ts_for_with_condition(head->tokenType);
  ck_assert_ulong_eq(head->childrenSize, 3);
  ck_assert_ptr_ne(head->children, NULL);
  variablesSection = head->children[TS_PARSE_FOR_VARIABLES];
  ck_assert_eq_ts_loop_variables_section(variablesSection->tokenType);
  conditionSection = head->children[TS_PARSE_FOR_CONDITION];
  ck_assert_eq_ts_loop_condition_section(conditionSection->tokenType);
  changeSection = head->children[TS_PARSE_FOR_CHANGE];
  ck_assert_eq_ts_loop_change_section(changeSection->tokenType);

  token = tsFile->tokens[1];
  ck_assert_ptr_ne(token, NULL);
  ck_assert_eq_ts_for(token->tokenType);
  ck_assert_ulong_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  head = token->children[0];
  ck_assert_eq_ts_for_with_condition(head->tokenType);
  ck_assert_ulong_eq(head->childrenSize, 3);
  ck_assert_ptr_ne(head->children, NULL);
  variablesSection = head->children[TS_PARSE_FOR_VARIABLES];
  ck_assert_eq_ts_loop_variables_section(variablesSection->tokenType);
  conditionSection = head->children[TS_PARSE_FOR_CONDITION];
  ck_assert_eq_ts_loop_condition_section(conditionSection->tokenType);
  changeSection = head->children[TS_PARSE_FOR_CHANGE];
  ck_assert_eq_ts_loop_change_section(changeSection->tokenType);

  body = token->children[1];
  ck_assert_eq_ts_scope(body->tokenType);
  ck_assert_ulong_eq(body->childrenSize, 0);
  ck_assert_ptr_eq(body->children, NULL);

  token = tsFile->tokens[2];
  ck_assert_ptr_ne(token, NULL);
  ck_assert_eq_ts_for(token->tokenType);
  ck_assert_ulong_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  head = token->children[0];
  ck_assert_eq_ts_for_with_condition(head->tokenType);
  ck_assert_ulong_eq(head->childrenSize, 3);
  ck_assert_ptr_ne(head->children, NULL);
  variablesSection = head->children[TS_PARSE_FOR_VARIABLES];
  ck_assert_eq_ts_loop_variables_section(variablesSection->tokenType);
  conditionSection = head->children[TS_PARSE_FOR_CONDITION];
  ck_assert_eq_ts_loop_condition_section(conditionSection->tokenType);
  changeSection = head->children[TS_PARSE_FOR_CHANGE];
  ck_assert_eq_ts_loop_change_section(changeSection->tokenType);

  body = token->children[1];
  ck_assert_eq_ts_scope(body->tokenType);
  ck_assert_ulong_eq(body->childrenSize, 1);
  ck_assert_ptr_ne(body->children, NULL);
  child = body->children[0];
  ck_assert_eq_ts_var(child->tokenType);

  TS_free_tsFile(tsFile);
END_TEST

void parse_for_let_suite(Suite *suite)
{
  TCase *tc_for_let = tcase_create("For with condition");

  tcase_add_test(tc_for_let, parse_valid_for_let);

  suite_add_tcase(suite, tc_for_let);
}