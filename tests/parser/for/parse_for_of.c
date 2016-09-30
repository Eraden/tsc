#include "./parse_for_of.h"

START_TEST(parse_valid_for_of)
  TSFile *tsFile = TS_parse_file("./examples/for/of/valid.ts");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_valid(tsFile);
  ck_assert_ptr_ne(tsFile->tokens, NULL);
  ck_assert_uint_eq(tsFile->tokensSize, 3);

  TSParserToken *token, *head, *body, *child;

  token = tsFile->tokens[0];
  ck_assert_ptr_ne(token, NULL);
  ck_assert_eq_ts_for(token->tokenType);
  ck_assert_ulong_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  head = token->children[0];
  ck_assert_eq_ts_for_of(head->tokenType);
  ck_assert_ulong_eq(head->childrenSize, 3);
  ck_assert_ptr_ne(head->children, NULL);

  token = tsFile->tokens[1];
  ck_assert_ptr_ne(token, NULL);
  ck_assert_eq_ts_for(token->tokenType);
  ck_assert_ulong_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  head = token->children[0];
  ck_assert_eq_ts_for_of(head->tokenType);
  ck_assert_ulong_eq(head->childrenSize, 3);
  ck_assert_ptr_ne(head->children, NULL);
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
  ck_assert_eq_ts_for_of(head->tokenType);
  ck_assert_ulong_eq(head->childrenSize, 3);
  ck_assert_ptr_ne(head->children, NULL);
  body = token->children[1];
  ck_assert_eq_ts_scope(body->tokenType);
  ck_assert_ulong_eq(body->childrenSize, 1);
  ck_assert_ptr_ne(body->children, NULL);
  child = body->children[0];
  ck_assert_eq_ts_const(child->tokenType);

  TS_free_tsFile(tsFile);
END_TEST

void parse_for_of_suite(Suite *suite)
{
  TCase *tc_for_of = tcase_create("For of");

  tcase_add_test(tc_for_of, parse_valid_for_of);

  suite_add_tcase(suite, tc_for_of);
}