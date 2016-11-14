#pragma once

#include <cts/sys.h>

#define _ck_assert_wstr(X, OP, Y) do { \
  const wchar_t* _wck_x = (X); \
  const wchar_t* _wck_y = (Y); \
  ck_assert_msg( 0 OP wcscmp(_wck_y, _wck_x), \
    "Assertion '%s' failed: %s == \"%ls\", %s == \"%ls\"", \
    #X" "#OP" "#Y, \
    #X, _wck_x, \
    #Y, _wck_y \
  ); \
} while (0)

#define ck_assert_wstr_eq(X, Y) _ck_assert_wstr(X, ==, Y)
#define ck_assert_wstr_ne(X, Y) _ck_assert_wstr(X, !=, Y)
#define ck_assert_wstr_lt(X, Y) _ck_assert_wstr(X, <, Y)
#define ck_assert_wstr_le(X, Y) _ck_assert_wstr(X, <=, Y)
#define ck_assert_wstr_gt(X, Y) _ck_assert_wstr(X, >, Y)
#define ck_assert_wstr_ge(X, Y) _ck_assert_wstr(X, >=, Y)

#define _ck_assert_ulong(X, OP, Y) do { \
  u_long _ck_x = (X); \
  u_long _ck_y = (Y); \
  ck_assert_msg(_ck_x OP _ck_y, "Assertion '%s' failed: %s == %lu, %s == %lu", #X" "#OP" "#Y, #X, _ck_x, #Y, _ck_y); \
} while (0)

#define ck_assert_ulong_eq(X, Y) _ck_assert_ulong(X, ==, Y)
#define ck_assert_ulong_ne(X, Y) _ck_assert_ulong(X, !=, Y)
#define ck_assert_ulong_lt(X, Y) _ck_assert_ulong(X, <, Y)
#define ck_assert_ulong_le(X, Y) _ck_assert_ulong(X, <=, Y)
#define ck_assert_ulong_gt(X, Y) _ck_assert_ulong(X, >, Y)
#define ck_assert_ulong_ge(X, Y) _ck_assert_ulong(X, >=, Y)
