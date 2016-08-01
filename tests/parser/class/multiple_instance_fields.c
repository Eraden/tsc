#include "./multiple_instance_fields.h"

static void
__attribute__((visibility("hidden")))
parse_class_with_complex_fields(int __attribute__((unused)) c) {
  tcase_fn_start ("parse_class_with_complex_fields", __FILE__, __LINE__);
  TSFile tsFile = TS_parse_file("./examples/class_with_multiple_fields.ts");

  validate_ts_file(tsFile, 1, TS_CLASS);

  TSParserToken token = tsFile.tokens[0];

  ck_assert_int_eq(token.childrenSize, (24 * 4) + 4);

  TSParserToken child;
  u_long childIndex = 0;
  TSLocalVariableData *fieldData;

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_NONE);
  ck_assert(child.tokenType == TS_INLINE_COMMENT);
  ck_assert_str_eq(child.data, " undefined first");

  // 1
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_1");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_1");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_1");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_1");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 2
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_2");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_2");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_2");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_2");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 3
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_3");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_3");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_3");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_3");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 4
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_4");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_4");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_4");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_4");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 5
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_5");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_5");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_5");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_5");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 6
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_6");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_6");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_6");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_6");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // comment
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_NONE);
  ck_assert(child.tokenType == TS_INLINE_COMMENT);

  // 7
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_7");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_7");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_7");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_7");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 8
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_8");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_8");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_8");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_8");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 9
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_9");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_9");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_9");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_9");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 10
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_10");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_10");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_10");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_10");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 11
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_11");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_11");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_11");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_11");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 12
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_12");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_12");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_12");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_12");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // comment
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_NONE);
  ck_assert(child.tokenType == TS_INLINE_COMMENT);
  ck_assert_ptr_ne(child.data, NULL);
  ck_assert_str_eq(child.data, " protected first");

  // 13
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_13");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_13");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_13");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_13");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 14
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_14");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_14");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_14");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_14");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 15
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_15");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_15");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_15");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_15");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 16
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_16");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_16");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_16");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_16");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 17
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_17");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_17");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_17");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_17");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 18
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_18");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_18");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_18");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_18");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // comment
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_NONE);
  ck_assert(child.tokenType == TS_INLINE_COMMENT);
  ck_assert_ptr_ne(child.data, NULL);
  ck_assert_str_eq(child.data, " public first");

  // 19
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_19");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_19");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_19");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_19");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 20
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_20");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_20");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_20");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_20");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 21
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_21");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_21");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_21");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_21");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 22
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_22");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_22");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_22");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_22");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 23
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_23");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_23");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_23");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_23");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  // 24
  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PUBLIC);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "public_field_24");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PROTECTED);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "protected_field_24");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex++];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "undefined_field_24");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  child = token.children[childIndex];
  ck_assert(child.visibility == TS_VISIBILITY_PRIVATE);
  ck_assert(child.tokenType == TS_CLASS_FIELD);
  fieldData = child.data;
  ck_assert_ptr_ne(fieldData, NULL);
  ck_assert_str_eq(fieldData->name, "private_field_24");
  ck_assert_ptr_eq(fieldData->type, NULL);
  ck_assert_ptr_eq(fieldData->value, NULL);

  TS_free_tsFile(tsFile);
}

void parse_class_with_multiple_instance_fields_suite(TCase *tc_class) {
  tcase_add_test(tc_class, parse_class_with_complex_fields);
}