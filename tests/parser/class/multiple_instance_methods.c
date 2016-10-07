#include "./multiple_instance_methods.h"

START_TEST(parse_class_with_complex_methods)
  TSFile *tsFile = TS_parse_file("./examples/class/class_with_multiple_methods.ts");

  ck_assert_int_eq(tsFile->tokensSize, 1);

  TSParserToken *token = tsFile->tokens[0];
  ck_assert(token->tokenType == TS_CLASS);

  ck_assert_int_eq(token->childrenSize, (24 * 4) + 4);

  TSParserToken *child;
  u_long childIndex = 0;
  TSFunctionData *methodData;

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_NONE);
  ck_assert(child->tokenType == TS_INLINE_COMMENT);
  ck_assert_wstr_eq(child->name, L" undefined first");

  // 1
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_1");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_1");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_1");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_1");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 2
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_2");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_2");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_2");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_2");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 3
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_3");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_3");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_3");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_3");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 4
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_4");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_4");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_4");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_4");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 5
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_5");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_5");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_5");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_5");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 6
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_6");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_6");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_6");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_6");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // comment
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_NONE);
  ck_assert(child->tokenType == TS_INLINE_COMMENT);

  // 7
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_7");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_7");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_7");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_7");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 8
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_8");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_8");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_8");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_8");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 9
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_9");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_9");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_9");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_9");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 10
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_10");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_10");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_10");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_10");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 11
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_11");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_11");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_11");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_11");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 12
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_12");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_12");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_12");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_12");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // comment
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_NONE);
  ck_assert(child->tokenType == TS_INLINE_COMMENT);
  ck_assert_ptr_ne(child->data, NULL);
  ck_assert_wstr_eq(child->data, L" protected first");

  // 13
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_13");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_13");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_13");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_13");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 14
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_14");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_14");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_14");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_14");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 15
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_15");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_15");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_15");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_15");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 16
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_16");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_16");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_16");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_16");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 17
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_17");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_17");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_17");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_17");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 18
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_18");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_18");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_18");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_18");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // comment
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_NONE);
  ck_assert(child->tokenType == TS_INLINE_COMMENT);
  ck_assert_ptr_ne(child->data, NULL);
  ck_assert_wstr_eq(child->data, L" public first");

  // 19
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_19");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_19");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_19");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_19");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 20
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_20");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_20");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_20");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_20");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 21
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_21");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_21");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_21");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_21");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 22
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_22");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_22");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_22");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_22");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 23
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_23");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_23");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_23");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_23");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  // 24
  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PUBLIC);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"public_method_24");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PROTECTED);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"protected_method_24");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex++];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"undefined_method_24");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  child = token->children[childIndex];
  ck_assert(child->visibility == TS_MODIFIER_PRIVATE);
  ck_assert(child->tokenType == TS_CLASS_METHOD);
  methodData = child->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"private_method_24");
  ck_assert_ptr_eq(methodData->returnType, NULL);
  ck_assert_ptr_ne(child->children, NULL);

  TS_free_tsFile(tsFile);
END_TEST

void parse_class_with_multiple_instance_methods_suite(TCase *tc_class) {
  tcase_add_test(tc_class, parse_class_with_complex_methods);
}