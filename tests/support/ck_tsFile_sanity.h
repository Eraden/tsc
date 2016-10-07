#pragma once

#include <cts/parser.h>

static char *
__attribute__((__used__))
_TS_sanity_to_string(const TSFileSanity sanity) {
  switch (sanity) {
    case TS_FILE_VALID: return "TS_FILE_VALID";
    case TS_FILE_SYNTAX_ERROR: return "TS_FILE_SYNTAX_ERROR";
    case TS_FILE_NOT_FOUND: return "TS_FILE_NOT_FOUND";
    default: NULL;
  }
}

#define _ck_assert_sanity(X, OP, Y) { \
  const TSFileSanity a = X; \
  const TSFileSanity b = Y;\
  ck_assert_msg( \
    a == b, "Assertion '%s' failed: %s == %i, %s == %i", \
    #X" "#OP" "#Y, \
    _TS_sanity_to_string(a),\
    a,\
    _TS_sanity_to_string(b),\
    b\
  ); \
}

#define ck_assert_sanity_eq(X, Y) _ck_assert_sanity(X, =, Y)
#define ck_assert_tsFile_valid(file) ck_assert_sanity_eq(file->sanity, TS_FILE_VALID)
#define ck_assert_tsFile_syntax_error(file) ck_assert_sanity_eq(file->sanity, TS_FILE_SYNTAX_ERROR)