#pragma once

#include <cts/register.h>

#define TS_check_validate_borrow(borrow, validValue) {\
  ck_assert_eq_ts_borrow(borrow->tokenType); \
  ck_assert_ptr_ne(borrow->children, NULL); \
  ck_assert_uint_eq(borrow->childrenSize, 1); \
  if (validValue) ck_assert_ts_token_eq(borrow->children[0], validValue); \
}

#define TS_check_validate_op_with_2_borrows(children, variableA, op, variableB) { \
  TSParserToken *borrow; borrow = children[0]; \
  TS_check_validate_borrow(borrow, variableA); \
  children += 1; \
  borrow = children[0]; \
  TS_check_validate_borrow(borrow, op); \
  children += 1; \
  borrow = children[0]; \
  TS_check_validate_borrow(borrow, variableB); \
  children += 1; \
}

#define TS_check_validate_arithmetic TS_check_validate_op_with_2_borrows
#define TS_check_validate_relation TS_check_validate_op_with_2_borrows
#define TS_check_validate_logical TS_check_validate_op_with_2_borrows
#define TS_check_validate_bitwise TS_check_validate_op_with_2_borrows
