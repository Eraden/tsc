#include "./classes.h"
#include "./class/multiple_instance_methods.h"
#include "class/multiple_instance_fields.h"

START_TEST(parse_valid_classes_file)
  TSFile *tsFile = TS_parse_file("./examples/class/valid.ts");

  TSParserToken *classToken = NULL;
  TSParserToken *childToken = NULL;
  TSParserToken *semicolon = NULL;

  ck_assert_tsFile_valid(tsFile);
  ck_assert_int_eq(tsFile->tokensSize, 12);

  // 1
  classToken = tsFile->tokens[0];

  ck_assert_eq_ts_class(classToken->tokenType);
  ck_assert_ptr_ne(classToken->name, NULL);
  ck_assert_wstr_eq(classToken->name, L"EmptyClass");
  ck_assert_int_eq(classToken->childrenSize, 0);

  // 2
  classToken = tsFile->tokens[1];

  ck_assert_eq_ts_class(classToken->tokenType);
  ck_assert_ptr_ne(classToken->name, NULL);
  ck_assert_wstr_eq(classToken->name, L"ClassWithField");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert_eq_ts_class_field(childToken->tokenType);
  ck_assert(childToken->modifiers == TS_MODIFIER_PRIVATE);

  ck_assert_wstr_eq(childToken->name, L"field_1");
  TS_check_validate_borrow(childToken->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);

  // 3
  classToken = tsFile->tokens[2];

  ck_assert_eq_ts_class(classToken->tokenType);
  ck_assert_ptr_ne(classToken->name, NULL);
  ck_assert_wstr_eq(classToken->name, L"ClassWithPrivateField");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert_eq_ts_class_field(childToken->tokenType);
  ck_assert(childToken->modifiers == TS_MODIFIER_PRIVATE);

  ck_assert_wstr_eq(childToken->name, L"field_2");
  TS_check_validate_borrow(childToken->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);

  // 4
  classToken = tsFile->tokens[3];

  ck_assert_eq_ts_class(classToken->tokenType);
  ck_assert_ptr_ne(classToken->name, NULL);
  ck_assert_wstr_eq(classToken->name, L"ClassWithProtectedField");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert_eq_ts_class_field(childToken->tokenType);
  ck_assert(childToken->modifiers == TS_MODIFIER_PROTECTED);

  ck_assert_wstr_eq(childToken->name, L"field_3");
  TS_check_validate_borrow(childToken->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);

  // 5
  classToken = tsFile->tokens[4];

  ck_assert_eq_ts_class(classToken->tokenType);
  ck_assert_ptr_ne(classToken->name, NULL);
  ck_assert_wstr_eq(classToken->name, L"ClassWithPublicField");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert_eq_ts_class_field(childToken->tokenType);
  ck_assert(childToken->modifiers == TS_MODIFIER_PUBLIC);

  ck_assert_wstr_eq(childToken->name, L"field_4");
  TS_check_validate_borrow(childToken->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);

  // 6
  classToken = tsFile->tokens[5];

  ck_assert_eq_ts_class(classToken->tokenType);
  ck_assert_ptr_ne(classToken->name, NULL);
  ck_assert_wstr_eq(classToken->name, L"ClassWithMethod");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert_eq_ts_class_method(childToken->tokenType);
  ck_assert(childToken->modifiers == TS_MODIFIER_PRIVATE);

  ck_assert_wstr_eq(childToken->name, L"method_1");
  ck_assert_ptr_ne(childToken->children, NULL);
  ck_assert_uint_eq(childToken->childrenSize, 3);

  // 7
  classToken = tsFile->tokens[6];

  ck_assert_eq_ts_class(classToken->tokenType);
  ck_assert_ptr_ne(classToken->name, NULL);
  ck_assert_wstr_eq(classToken->name, L"ClassWithPrivateMethod");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert_eq_ts_class_method(childToken->tokenType);
  ck_assert(childToken->modifiers == TS_MODIFIER_PRIVATE);

  ck_assert_wstr_eq(childToken->name, L"method_2");
  ck_assert_ptr_ne(childToken->children, NULL);
  ck_assert_uint_eq(childToken->childrenSize, 3);

  // 8
  classToken = tsFile->tokens[7];

  ck_assert_eq_ts_class(classToken->tokenType);
  ck_assert_ptr_ne(classToken->name, NULL);
  ck_assert_wstr_eq(classToken->name, L"ClassWithProtectedMethod");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert_eq_ts_class_method(childToken->tokenType);
  ck_assert(childToken->modifiers == TS_MODIFIER_PROTECTED);

  ck_assert_wstr_eq(childToken->name, L"method_3");
  ck_assert_ptr_ne(childToken->children, NULL);
  ck_assert_uint_eq(childToken->childrenSize, 3);

  // 9
  classToken = tsFile->tokens[8];

  ck_assert_eq_ts_class(classToken->tokenType);
  ck_assert_ptr_ne(classToken->name, NULL);
  ck_assert_wstr_eq(classToken->name, L"ClassWithPublicMethod");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert_eq_ts_class_method(childToken->tokenType);
  ck_assert(childToken->modifiers == TS_MODIFIER_PUBLIC);

  ck_assert_wstr_eq(childToken->name, L"method_4");
  ck_assert_ptr_ne(childToken->children, NULL);
  ck_assert_uint_eq(childToken->childrenSize, 3);

  // 10
  classToken = tsFile->tokens[9];

  ck_assert_eq_ts_class(classToken->tokenType);
  ck_assert_ptr_ne(classToken->name, NULL);
  ck_assert_wstr_eq(classToken->name, L"ClassWithFieldAndMethod");
  ck_assert_int_eq(classToken->childrenSize, 2);

  childToken = classToken->children[0];
  ck_assert_eq_ts_class_field(childToken->tokenType);
  ck_assert(childToken->modifiers == TS_MODIFIER_PRIVATE);

  ck_assert_wstr_eq(childToken->name, L"foo");
  TS_check_validate_borrow(childToken->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);

  childToken = classToken->children[1];
  ck_assert_eq_ts_class_method(childToken->tokenType);
  ck_assert(childToken->modifiers == TS_MODIFIER_PRIVATE);

  ck_assert_wstr_eq(childToken->name, L"bar");
  ck_assert_ptr_ne(childToken->children, NULL);
  ck_assert_uint_eq(childToken->childrenSize, 3);

  // 11
  TSParserToken *token = tsFile->tokens[10];
  ck_assert_eq_ts_const(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"TEST");
  ck_assert_uint_eq(token->childrenSize, 2);
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);
  TSParserToken *variableValue = token->children[TS_VARIABLE_VALUE_INDEX];
  ck_assert_eq_ts_class(variableValue->tokenType);

  // 12
  semicolon = tsFile->tokens[11];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  TS_free_ts_file(tsFile);
END_TEST


// Errors

START_TEST(parse_class_with_invalid_name)
  TSFile *file = TS_parse_file("./examples/class/invalid_class_name");
  ck_assert_ptr_ne(file, NULL);
  ck_assert(file->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_class_without_name)
  TSFile *file = TS_parse_file("./examples/class/missing_class_name");
  ck_assert_ptr_ne(file, NULL);
  ck_assert(file->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_class_with_multiple_super_class)
  TSFile *file = TS_parse_file("./examples/class/multiple_super_class");
  ck_assert_ptr_ne(file, NULL);
  ck_assert(file->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_class_member_without_ending)
  TSFile *file = TS_parse_file("./examples/class/missing_class_ending");
  ck_assert_ptr_ne(file, NULL);
  ck_assert(file->sanity == TS_FILE_SYNTAX_ERROR);
  TS_free_ts_file(file);
END_TEST

void parse_classes_suite(Suite *suite) {
  TCase *tc_class = tcase_create("Class");
  tcase_add_test(tc_class, parse_valid_classes_file);

  parse_class_with_multiple_instance_fields_suite(tc_class);
  parse_class_with_multiple_instance_methods_suite(tc_class);

  tcase_add_test(tc_class, parse_class_with_invalid_name);
  tcase_add_test(tc_class, parse_class_without_name);
  tcase_add_test(tc_class, parse_class_with_multiple_super_class);
  tcase_add_test(tc_class, parse_class_member_without_ending);

  suite_add_tcase(suite, tc_class);
}