#include "./operators.h"

START_TEST(check_valid)
  TSFile *tsFile = TS_parse_file("./examples/operators/valid.ts");
  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 1);

  TSParserToken *scope, *token, *variableA, *variableB, *op, *borrow;
  TSParserToken **children = NULL;

  scope = tsFile->tokens[0];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_uint_eq(scope->childrenSize, 59);
  children = scope->children;

  token = variableA = children[0];
  ck_assert_eq_ts_var(token->tokenType);
  children += 1;

  token = variableB = children[0];
  ck_assert_eq_ts_var(token->tokenType);
  children += 1;

  // a + b
  TS_check_validate_arithmetic(children, variableA, TS_PLUS, variableB);
  //  a - b;
  TS_check_validate_arithmetic(children, variableA, TS_MINUS, variableB);
  //  a * b;
  TS_check_validate_arithmetic(children, variableA, TS_MULTIPLY, variableB);
  //  a / b;
  TS_check_validate_arithmetic(children, variableA, TS_DIVIDE, variableB);
  //  a % b;
  TS_check_validate_arithmetic(children, variableA, TS_MODULO, variableB);

  //  a == b;
  TS_check_validate_relation(children, variableA, TS_EQUAL, variableB);
  //  a === b;
  TS_check_validate_relation(children, variableA, TS_SAME, variableB);
  //  a != b;
  TS_check_validate_relation(children, variableA, TS_NOT_EQUAL, variableB);
  //  a > b;
  TS_check_validate_relation(children, variableA, TS_GREATER, variableB);
  //  a >= b;
  TS_check_validate_relation(children, variableA, TS_GREATER_OR_EQUAL, variableB);
  //  a < b;
  TS_check_validate_relation(children, variableA, TS_LESS, variableB);
  //  a <= b;
  TS_check_validate_relation(children, variableA, TS_LESS_OR_EQUAL, variableB);

  //  a && b;
  TS_check_validate_logical(children, variableA, TS_LOGICAL_AND, variableB);
  //  a || b;
  TS_check_validate_logical(children, variableA, TS_LOGICAL_OR, variableB);

  //  a & b;
  TS_check_validate_bitwise(children, variableA, TS_BITWISE_AND, variableB);
  //  a | b;
  TS_check_validate_bitwise(children, variableA, TS_BITWISE_OR, variableB);
  //  a ^ b;
  TS_check_validate_bitwise(children, variableA, TS_BITWISE_XOR, variableB);

  //  !a;
  op = children[0];
  TS_check_validate_borrow(op, TS_LOGICAL_NOT);
  children += 1;
  borrow = children[0];
  TS_check_validate_borrow(borrow, variableA);
  children += 1;
  //  a++;
  borrow = children[0];
  TS_check_validate_borrow(borrow, variableA);
  children += 1;
  op = children[0];
  TS_check_validate_borrow(op, TS_INCREMENT);
  children += 1;
  //  a--;
  borrow = children[0];
  TS_check_validate_borrow(borrow, variableA);
  children += 1;
  op = children[0];
  TS_check_validate_borrow(op, TS_DECREMENT);

  TS_free_tsFile(tsFile);
END_TEST

void parse_operators_suite(Suite *suite) {
  TCase *tc_op = tcase_create("operators");
  tcase_add_test(tc_op, check_valid);
  suite_add_tcase(suite, tc_op);
}