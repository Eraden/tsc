#pragma once

#include <tsc/sys.h>

#define _ck_assert_wstr(X, OP, Y) do { \
  const wchar_t* _ck_x = (X); \
  const wchar_t* _ck_y = (Y); \
  ck_assert_msg( 0 OP wcscmp(_ck_y, _ck_x), \
    "Assertion '%s' failed: %ls == \"%ls\", %ls == \"%ls\"", #X" "#OP" "#Y, #X, _ck_x, #Y, _ck_y); \
} while (0)

#define ck_assert_wstr_eq(X, Y) _ck_assert_wstr(X, ==, Y)
#define ck_assert_wstr_ne(X, Y) _ck_assert_wstr(X, !=, Y)
#define ck_assert_wstr_lt(X, Y) _ck_assert_wstr(X, <, Y)
#define ck_assert_wstr_le(X, Y) _ck_assert_wstr(X, <=, Y)
#define ck_assert_wstr_gt(X, Y) _ck_assert_wstr(X, >, Y)
#define ck_assert_wstr_ge(X, Y) _ck_assert_wstr(X, >=, Y)