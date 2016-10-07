#pragma once

#include <cts/register.h>

#define _ck_assert_ts_token(X, OP, Y) do { \
  TSParserToken* _ck_x = (X); \
  TSParserToken* _ck_y = (Y); \
  ck_assert_msg(_ck_x OP _ck_y, "Assertion '%s' failed: %s == %lu, %s == %lu", #X" "#OP" "#Y, #X, _ck_x, #Y, _ck_y); \
} while (0)

#define ck_assert_ts_token_eq(X, Y) _ck_assert_ts_token(X, ==, Y)
#define ck_assert_ts_token_ne(X, Y) _ck_assert_ts_token(X, !=, Y)
#define ck_assert_ts_token_lt(X, Y) _ck_assert_ts_token(X, <, Y)
#define ck_assert_ts_token_le(X, Y) _ck_assert_ts_token(X, <=, Y)
#define ck_assert_ts_token_gt(X, Y) _ck_assert_ts_token(X, >, Y)
#define ck_assert_ts_token_ge(X, Y) _ck_assert_ts_token(X, >=, Y)