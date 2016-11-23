#pragma once

#include <cts/register.h>
#include "./ck_token_type.h"
#include <check.h>

static void _ck_validate_child(TSParserToken *child, TSTokenType type, const wchar_t *name, unsigned int size) {
  ck_assert_ptr_ne(child, NULL);
  ck_assert_tsType_eq(child->tokenType, type);
  if (((const wchar_t *) name) != NULL) {
    ck_assert_ptr_ne(child->name, NULL);
    ck_assert_wstr_eq(child->name, (const wchar_t *) name);
  } else
    ck_assert_ptr_eq(child->name, NULL);
  ck_assert_uint_eq(child->childrenSize, size);
  if (size) {
    ck_assert_ptr_ne(child->children, NULL);
  } else
    ck_assert_ptr_eq(child->children, NULL);
}

#define ck_validate_child( child, type, name, size ) _ck_validate_child( child, type, (const wchar_t *) name, size );