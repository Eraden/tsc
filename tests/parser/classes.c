#include "./classes.h"
#include "./class/multiple_instance_methods.h"
#include "class/multiple_instance_fields.h"

START_TEST(parse_valid_classes_file)
  TSFile *tsFile = TS_parse_file("./examples/class/valid.ts");

  TSParserToken *classToken;
  TSParserToken *childToken;
  TSClassData *classData;
  TSFunctionData *methodData;

  ck_assert_int_eq(tsFile->tokensSize, 11);

  // 1
  classToken = tsFile->tokens[0];

  ck_assert(classToken->tokenType == TS_CLASS);
  classData = classToken->classData;

  ck_assert_ptr_ne(classData, NULL);
  ck_assert_ptr_ne(classData->name, NULL);
  ck_assert_wstr_eq(classData->name, L"EmptyClass");
  ck_assert_int_eq(classToken->childrenSize, 0);

  // 2
  classToken = tsFile->tokens[1];

  ck_assert(classToken->tokenType == TS_CLASS);
  classData = classToken->classData;

  ck_assert_ptr_ne(classData, NULL);
  ck_assert_ptr_ne(classData->name, NULL);
  ck_assert_wstr_eq(classData->name, L"ClassWithField");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert(childToken->tokenType == TS_CLASS_FIELD);
  ck_assert(childToken->visibility == TS_VISIBILITY_PRIVATE);

  TSLocalVariableData *childData = childToken->data;
  ck_assert_ptr_ne(childData, NULL);
  ck_assert_wstr_eq(childData->name, L"field_1");
  ck_assert_ptr_eq(childData->type, NULL);
  ck_assert_ptr_eq(childData->value, NULL);

  // 3
  classToken = tsFile->tokens[2];

  ck_assert(classToken->tokenType == TS_CLASS);
  classData = classToken->data;

  ck_assert_ptr_ne(classData, NULL);
  ck_assert_ptr_ne(classData->name, NULL);
  ck_assert_wstr_eq(classData->name, L"ClassWithPrivateField");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert(childToken->tokenType == TS_CLASS_FIELD);
  ck_assert(childToken->visibility == TS_VISIBILITY_PRIVATE);

  childData = childToken->data;
  ck_assert_ptr_ne(childData, NULL);
  ck_assert_wstr_eq(childData->name, L"field_2");
  ck_assert_ptr_eq(childData->type, NULL);
  ck_assert_ptr_eq(childData->value, NULL);

  // 4
  classToken = tsFile->tokens[3];

  ck_assert(classToken->tokenType == TS_CLASS);
  classData = classToken->data;

  ck_assert_ptr_ne(classData, NULL);
  ck_assert_ptr_ne(classData->name, NULL);
  ck_assert_wstr_eq(classData->name, L"ClassWithProtectedField");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert(childToken->tokenType == TS_CLASS_FIELD);
  ck_assert(childToken->visibility == TS_VISIBILITY_PROTECTED);

  childData = childToken->data;
  ck_assert_ptr_ne(childData, NULL);
  ck_assert_wstr_eq(childData->name, L"field_3");;
  ck_assert_ptr_eq(childData->type, NULL);
  ck_assert_ptr_eq(childData->value, NULL);

  // 5
  classToken = tsFile->tokens[4];

  ck_assert(classToken->tokenType == TS_CLASS);
  classData = classToken->data;

  ck_assert_ptr_ne(classData, NULL);
  ck_assert_ptr_ne(classData->name, NULL);
  ck_assert_wstr_eq(classData->name, L"ClassWithPublicField");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert(childToken->tokenType == TS_CLASS_FIELD);
  ck_assert(childToken->visibility == TS_VISIBILITY_PUBLIC);

  childData = childToken->data;
  ck_assert_ptr_ne(childData, NULL);
  ck_assert_wstr_eq(childData->name, L"field_4");;
  ck_assert_ptr_eq(childData->type, NULL);
  ck_assert_ptr_eq(childData->value, NULL);

  // 6
  classToken = tsFile->tokens[5];

  ck_assert(classToken->tokenType == TS_CLASS);
  classData = classToken->data;

  ck_assert_ptr_ne(classData, NULL);
  ck_assert_ptr_ne(classData->name, NULL);
  ck_assert_wstr_eq(classData->name, L"ClassWithMethod");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert(childToken->tokenType == TS_CLASS_METHOD);
  ck_assert(childToken->visibility == TS_VISIBILITY_PRIVATE);

  methodData = childToken->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"method_1");
  ck_assert_ptr_eq(childToken->children, NULL);
  ck_assert_uint_eq(childToken->childrenSize, 0);
  ck_assert_ptr_eq(methodData->returnType, NULL);

  // 7
  classToken = tsFile->tokens[6];

  ck_assert(classToken->tokenType == TS_CLASS);
  classData = classToken->data;

  ck_assert_ptr_ne(classData, NULL);
  ck_assert_ptr_ne(classData->name, NULL);
  ck_assert_wstr_eq(classData->name, L"ClassWithPrivateMethod");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert(childToken->tokenType == TS_CLASS_METHOD);
  ck_assert(childToken->visibility == TS_VISIBILITY_PRIVATE);

  methodData = childToken->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"method_2");;
  ck_assert_ptr_eq(childToken->children, NULL);
  ck_assert_uint_eq(childToken->childrenSize, 0);
  ck_assert_ptr_eq(methodData->returnType, NULL);

  // 8
  classToken = tsFile->tokens[7];

  ck_assert(classToken->tokenType == TS_CLASS);
  classData = classToken->data;

  ck_assert_ptr_ne(classData, NULL);
  ck_assert_ptr_ne(classData->name, NULL);
  ck_assert_wstr_eq(classData->name, L"ClassWithProtectedMethod");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert(childToken->tokenType == TS_CLASS_METHOD);
  ck_assert(childToken->visibility == TS_VISIBILITY_PROTECTED);

  methodData = childToken->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"method_3");
  ck_assert_ptr_eq(childToken->children, NULL);
  ck_assert_uint_eq(childToken->childrenSize, 0);
  ck_assert_ptr_eq(methodData->returnType, NULL);

  // 9
  classToken = tsFile->tokens[8];

  ck_assert(classToken->tokenType == TS_CLASS);
  classData = classToken->data;

  ck_assert_ptr_ne(classData, NULL);
  ck_assert_ptr_ne(classData->name, NULL);
  ck_assert_wstr_eq(classData->name, L"ClassWithPublicMethod");
  ck_assert_int_eq(classToken->childrenSize, 1);

  childToken = classToken->children[0];
  ck_assert(childToken->tokenType == TS_CLASS_METHOD);
  ck_assert(childToken->visibility == TS_VISIBILITY_PUBLIC);

  methodData = childToken->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"method_4");
  ck_assert_ptr_eq(childToken->children, NULL);
  ck_assert_uint_eq(childToken->childrenSize, 0);
  ck_assert_ptr_eq(methodData->returnType, NULL);

  // 10
  classToken = tsFile->tokens[9];

  ck_assert(classToken->tokenType == TS_CLASS);
  classData = classToken->data;

  ck_assert_ptr_ne(classData, NULL);
  ck_assert_ptr_ne(classData->name, NULL);
  ck_assert_wstr_eq(classData->name, L"ClassWithFieldAndMethod");
  ck_assert_int_eq(classToken->childrenSize, 2);

  childToken = classToken->children[0];
  ck_assert(childToken->tokenType == TS_CLASS_FIELD);
  ck_assert(childToken->visibility == TS_VISIBILITY_PRIVATE);

  childData = childToken->data;
  ck_assert_ptr_ne(childData, NULL);
  ck_assert_wstr_eq(childData->name, L"foo");
  ck_assert_ptr_eq(childData->type, NULL);
  ck_assert_ptr_eq(childData->value, NULL);

  childToken = classToken->children[1];
  ck_assert(childToken->tokenType == TS_CLASS_METHOD);
  ck_assert(childToken->visibility == TS_VISIBILITY_PRIVATE);

  methodData = childToken->data;
  ck_assert_ptr_ne(methodData, NULL);
  ck_assert_wstr_eq(methodData->name, L"bar");
  ck_assert_ptr_eq(childToken->children, NULL);
  ck_assert_uint_eq(childToken->childrenSize, 0);
  ck_assert_ptr_eq(methodData->returnType, NULL);

  // 11
  TSParserToken *token = tsFile->tokens[10];
  ck_assert(token->tokenType == TS_CONST);
  ck_assert_ptr_ne(token->variableData, NULL);
  ck_assert_ptr_ne(token->variableData->name, NULL);
  ck_assert_wstr_eq(token->variableData->name, L"TEST");
  ck_assert_ptr_eq(token->variableData->type, NULL);
  ck_assert_ptr_ne(token->variableData->value, NULL);
  ck_assert_wstr_eq(token->variableData->value, L"class{}");

  TS_free_tsFile(tsFile);
END_TEST


// Errors

START_TEST(parse_class_with_invalid_name)
  TS_parse_file("./examples/class/invalid_class_name");
END_TEST

START_TEST(parse_class_without_name)
  TS_parse_file("./examples/class/missing_class_name");
END_TEST

START_TEST(parse_class_with_multiple_super_class)
  TS_parse_file("./examples/class/multiple_super_class");
END_TEST

START_TEST(parse_class_member_without_ending)
  TS_parse_file("./examples/class/missing_class_ending");
END_TEST

void parse_classes_suite(Suite *suite) {
  TCase *tc_class = tcase_create("Class");
  tcase_add_test(tc_class, parse_valid_classes_file);

  parse_class_with_multiple_instance_fields_suite(tc_class);
  parse_class_with_multiple_instance_methods_suite(tc_class);

  tcase_add_exit_test(tc_class, parse_class_with_invalid_name, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_class, parse_class_without_name, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_class, parse_class_with_multiple_super_class, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_class, parse_class_member_without_ending, TS_PARSE_FAILURE_CODE);

  suite_add_tcase(suite, tc_class);
}