#include "./operators.h"

START_TEST(check_valid)
  TSFile *tsFile = TS_parse_file("./examples/operators/valid.ts");
  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 1);

  TSParserToken *scope, *token, *variableA, *variableB, *op, *borrow;
  TSParserToken **children = NULL;

  const unsigned short int semicolonsCount = 22;
  const unsigned short int variablesCount = 2;

  scope = tsFile->tokens[0];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_uint_eq(scope->childrenSize, 57 + variablesCount + semicolonsCount);
  children = scope->children;

  token = variableA = children[0];
  ck_assert_eq_ts_var(token->tokenType);
  children += 1;

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;

  token = variableB = children[0];
  ck_assert_eq_ts_var(token->tokenType);
  children += 1;

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;

  // a + b
  TS_check_validate_arithmetic(children, variableA, TS_PLUS, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a - b;
  TS_check_validate_arithmetic(children, variableA, TS_MINUS, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a * b;
  TS_check_validate_arithmetic(children, variableA, TS_MULTIPLY, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a / b;
  TS_check_validate_arithmetic(children, variableA, TS_DIVIDE, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a % b;
  TS_check_validate_arithmetic(children, variableA, TS_MODULO, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;

  //  a == b;
  TS_check_validate_relation(children, variableA, TS_EQUAL, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a === b;
  TS_check_validate_relation(children, variableA, TS_SAME, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a != b;
  TS_check_validate_relation(children, variableA, TS_NOT_EQUAL, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a > b;
  TS_check_validate_relation(children, variableA, TS_GREATER, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a >= b;
  TS_check_validate_relation(children, variableA, TS_GREATER_OR_EQUAL, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a < b;
  TS_check_validate_relation(children, variableA, TS_LESS, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a <= b;
  TS_check_validate_relation(children, variableA, TS_LESS_OR_EQUAL, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;

  //  a && b;
  TS_check_validate_logical(children, variableA, TS_LOGICAL_AND, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a || b;
  TS_check_validate_logical(children, variableA, TS_LOGICAL_OR, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;

  //  a & b;
  TS_check_validate_bitwise(children, variableA, TS_BITWISE_AND, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a | b;
  TS_check_validate_bitwise(children, variableA, TS_BITWISE_OR, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a ^ b;
  TS_check_validate_bitwise(children, variableA, TS_BITWISE_XOR, variableB);

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;

  //  !a;
  op = children[0];
  TS_check_validate_borrow(op, TS_LOGICAL_NOT);
  children += 1;
  borrow = children[0];
  TS_check_validate_borrow(borrow, variableA);
  children += 1;

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a++;
  borrow = children[0];
  TS_check_validate_borrow(borrow, variableA);
  children += 1;
  op = children[0];
  TS_check_validate_borrow(op, TS_INCREMENT);
  children += 1;

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
  children += 1;
  //  a--;
  borrow = children[0];
  TS_check_validate_borrow(borrow, variableA);
  children += 1;
  op = children[0];
  TS_check_validate_borrow(op, TS_DECREMENT);
  children += 1;

  token = children[0];
  ck_assert_eq_ts_semicolon(token->tokenType);
//  children += 1;

  TS_free_tsFile(tsFile);
END_TEST

void parse_operators_suite(Suite *suite) {
  TCase *tc_op = tcase_create("operators");
  tcase_add_test(tc_op, check_valid);
  suite_add_tcase(suite, tc_op);
}