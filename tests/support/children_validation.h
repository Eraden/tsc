#pragma once

#include <cts/register.h>
#include "./ck_token_type.h"
#include <check.h>

#define ck_validate_child(child, type, NAME, size) \
  { \
    const wchar_t *__name = (const wchar_t *) NAME; \
    ck_assert_ptr_ne(child, NULL); \
    ck_assert_tsType_eq(child->tokenType, type); \
    if (__name != NULL) ck_assert_ptr_ne(child->name, NULL); \
    if (__name != NULL) ck_assert_wstr_eq(child->name, (const wchar_t *) __name); \
    if (__name == NULL) ck_assert_ptr_eq(child->name, NULL); \
    ck_assert_uint_eq(child->childrenSize, size); \
    if (size) ck_assert_ptr_ne(child->children, NULL); else ck_assert_ptr_eq(child->children, NULL); \
  }

#define ck_validate_extends(CLASS, PARENT) \
  { \
    TSParserToken *__classToken = CLASS; \
    TSParserToken *__givenParent = PARENT; \
    ck_assert_uint_gt(__classToken->childrenSize, 0); \
    ck_assert_ptr_ne(__classToken->children, NULL); \
    TSParserToken *__extends = __classToken->children[0]; \
    ck_validate_child(__extends, TS_EXTENDS, NULL, 1); \
    TSParserToken *__borrow = __extends->children[0]; \
    ck_validate_child(__borrow, TS_BORROW, NULL, 1); \
    TSParserToken *__parent = __borrow->children[0]; \
    ck_assert_ptr_eq(__parent, __givenParent); \
  }

#define TS_begin_structural_validation(TOKEN, TYPE, NAME, SIZE) { \
  TSParserToken * TOKEN ## __token = TOKEN; \
  const wchar_t * TOKEN ## __name = (const wchar_t *) NAME; \
  TSTokenType TOKEN ## __type = TYPE; \
  unsigned int TOKEN ## __size = SIZE; \
  ck_validate_child( TOKEN ## __token, TOKEN ## __type, TOKEN ## __name, TOKEN ## __size); \
  TSParserToken ** TOKEN ## __children = ( TOKEN ## __token )->children; \
  TSParserToken * TOKEN ## __current = NULL; \
  if ( TOKEN ## __size ) TOKEN ## __current = TOKEN ## __children [0];

#define TS_end_structural_validation() }

#define TS_begin_child_structural_validation( AS, TYPE, NAME, SIZE, FROM) \
  TSParserToken * AS = FROM ## __current; \
  TS_begin_structural_validation( AS, TYPE, NAME, SIZE)

#define TS_end_child_structural_validation(TOKEN, HAS_NEXT) \
  unsigned int hasNext = HAS_NEXT; \
  if (hasNext) { \
    TOKEN ## __children += 1;  \
    TOKEN ## __current = TOKEN ## __children [ 0 ]; \
  } \
  TS_end_structural_validation()

#define ck_begin_token(TOKEN, TYPE, NAME, SIZE) TS_begin_structural_validation(TOKEN, TYPE, NAME, SIZE)
#define ck_end_token() TS_end_structural_validation()
#define ck_begin_child( AS, TYPE, NAME, SIZE, FROM ) TS_begin_child_structural_validation( AS, TYPE, NAME, SIZE, FROM )
#define ck_end_child(TOKEN, HAS_NEXT) TS_end_child_structural_validation(TOKEN, HAS_NEXT)
