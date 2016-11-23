#pragma once

#include <cts/register.h>
#include <check.h>

#define _ck_validate_child(child, type, name, size) \
{ \
  ck_assert_ptr_ne(child, NULL); \
  ck_assert_tsToken_eq(child->tokenType, type); \
  if (name) { \
    ck_assert_ptr_ne(child->name, NULL); \
    ck_assert_wstr_eq(child->name, name); \
  } else ck_assert_ptr_eq(child->name, NULL); \
  ck_assert_uint_eq(child->childrenSize, size); \
  if (size) { \
    ck_assert_ptr_ne(child->children, NULL); \
  } else ck_assert_ptr_eq(child->children, NULL); \
}

#define ck_validate_child(child, type, name, size) _ck_validate_child(child, type, (const wchar_t *) name, (unsigned int) size)