#pragma once

#include <cts/sys.h>

#define _ck_assert_wcs_included(expected, current, result) do { \
  const wchar_t* _expected = expected; \
  const wchar_t* _current = current; \
  const wchar_t* _result = result; \
  ck_assert_msg(_result != NULL, "Assertion '%s' failed: %s == %lu, %s == %lu (\"%s\" == \"%ls\")", \
    #current" include "#expected, \
    #current, _current, \
    #expected, _expected, \
    result, _result); \
} while (0)

#define TS_check_error(tsFile, error_message) {\
  ck_assert_tsFile_syntax_error(tsFile); \
  ck_assert_ptr_ne(tsFile->errorReason, NULL); \
  const wchar_t *_error_message = error_message; \
  const wchar_t *found_error = wcsstr(tsFile->errorReason, _error_message); \
  _ck_assert_wcs_included(_error_message, tsFile->errorReason, found_error); \
}