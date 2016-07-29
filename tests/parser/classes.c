#include "./classes.h"

START_TEST(parsing_empty_class)
  {
    const char content[] = "class Empty {}";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_str_eq(classData->name, "Empty");
    ck_assert_int_eq(classToken.childrenSize, 0);
  }
END_TEST

// fields

START_TEST(parse_class_with_undefined_field)
  {
    const char content[] = "class Empty {\n  field_1;\n}";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_int_eq(classToken.childrenSize, 1);

    TSParserToken childToken;

    childToken = classToken.children[0];
    ck_assert(childToken.tokenType == TS_CLASS_FIELD);
    ck_assert(childToken.visibility == TS_VISIBILITY_PRIVATE);

    TSLocalVariableData *childData = childToken.data;
    ck_assert_ptr_ne(childData, NULL);
    ck_assert_str_eq(childData->name, "field_1");
    ck_assert_ptr_eq(childData->type, NULL);
    ck_assert_ptr_eq(childData->value, NULL);
  }
END_TEST

START_TEST(parse_class_with_private_field)
  {
    const char content[] = "class Empty {\n  private field_2;\n}";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_int_eq(classToken.childrenSize, 1);

    TSParserToken childToken;

    childToken = classToken.children[0];
    ck_assert(childToken.tokenType == TS_CLASS_FIELD);
    ck_assert(childToken.visibility == TS_VISIBILITY_PRIVATE);

    TSLocalVariableData *childData = childToken.data;
    ck_assert_ptr_ne(childData, NULL);
    ck_assert_str_eq(childData->name, "field_2");
    ck_assert_ptr_eq(childData->type, NULL);
    ck_assert_ptr_eq(childData->value, NULL);
  }
END_TEST

START_TEST(parse_class_with_protected_field)
  {
    const char content[] = "class Empty {\n  protected field_3;\n}";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_int_eq(classToken.childrenSize, 1);

    TSParserToken childToken;

    childToken = classToken.children[0];
    ck_assert(childToken.tokenType == TS_CLASS_FIELD);
    ck_assert(childToken.visibility == TS_VISIBILITY_PROTECTED);

    TSLocalVariableData *childData = childToken.data;
    ck_assert_ptr_ne(childData, NULL);
    ck_assert_str_eq(childData->name, "field_3");
    ck_assert_ptr_eq(childData->type, NULL);
    ck_assert_ptr_eq(childData->value, NULL);
  }
END_TEST

START_TEST(parse_class_with_public_field)
  {
    const char content[] = "class Empty {\n  public field_4;\n}";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_int_eq(classToken.childrenSize, 1);

    TSParserToken childToken;

    childToken = classToken.children[0];
    ck_assert(childToken.tokenType == TS_CLASS_FIELD);
    ck_assert(childToken.visibility == TS_VISIBILITY_PUBLIC);

    TSLocalVariableData *childData = childToken.data;
    ck_assert_ptr_ne(childData, NULL);
    ck_assert_str_eq(childData->name, "field_4");
    ck_assert_ptr_eq(childData->type, NULL);
    ck_assert_ptr_eq(childData->value, NULL);
  }
END_TEST

// methods

START_TEST(parse_class_with_undefined_method)
  {
    const char content[] = "class Empty {\n  method_1() {}\n}";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_int_eq(classToken.childrenSize, 1);

    TSParserToken childToken;
    childToken = classToken.children[0];
    ck_assert(childToken.tokenType == TS_CLASS_METHOD);
    ck_assert(childToken.visibility == TS_VISIBILITY_PRIVATE);

    TSFunctionData *methodData = childToken.data;
    ck_assert_ptr_ne(methodData, NULL);
    ck_assert_str_eq(methodData->name, "method_1");
    ck_assert_ptr_eq(methodData->arguments, NULL);
    ck_assert_int_eq(methodData->argumentsSize, 0);
    ck_assert_ptr_eq(methodData->returnType, NULL);
  }
END_TEST

START_TEST(parse_class_with_private_method)
  {
    const char content[] = "class Empty {\n  private method_2() {}\n}";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_int_eq(classToken.childrenSize, 1);

    TSParserToken childToken;
    childToken = classToken.children[0];
    ck_assert(childToken.tokenType == TS_CLASS_METHOD);
    ck_assert(childToken.visibility == TS_VISIBILITY_PRIVATE);

    TSFunctionData *methodData = childToken.data;
    ck_assert_ptr_ne(methodData, NULL);
    ck_assert_str_eq(methodData->name, "method_2");
    ck_assert_ptr_eq(methodData->arguments, NULL);
    ck_assert_int_eq(methodData->argumentsSize, 0);
    ck_assert_ptr_eq(methodData->returnType, NULL);
  }
END_TEST

START_TEST(parse_class_with_protected_method)
  {
    const char content[] = "class Empty {\n  protected method_3() {}\n}";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_int_eq(classToken.childrenSize, 1);

    TSParserToken childToken;
    childToken = classToken.children[0];
    ck_assert(childToken.tokenType == TS_CLASS_METHOD);
    ck_assert(childToken.visibility == TS_VISIBILITY_PROTECTED);

    TSFunctionData *methodData = childToken.data;
    ck_assert_ptr_ne(methodData, NULL);
    ck_assert_str_eq(methodData->name, "method_3");
    ck_assert_ptr_eq(methodData->arguments, NULL);
    ck_assert_int_eq(methodData->argumentsSize, 0);
    ck_assert_ptr_eq(methodData->returnType, NULL);
  }
END_TEST

START_TEST(parse_class_with_public_method)
  {
    const char content[] = "class Empty {\n  public method_3() {}\n}";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_int_eq(classToken.childrenSize, 1);

    TSParserToken childToken;
    childToken = classToken.children[0];
    ck_assert(childToken.tokenType == TS_CLASS_METHOD);
    ck_assert(childToken.visibility == TS_VISIBILITY_PUBLIC);

    TSFunctionData *methodData = childToken.data;
    ck_assert_ptr_ne(methodData, NULL);
    ck_assert_str_eq(methodData->name, "method_3");
    ck_assert_ptr_eq(methodData->arguments, NULL);
    ck_assert_int_eq(methodData->argumentsSize, 0);
    ck_assert_ptr_eq(methodData->returnType, NULL);
  }
END_TEST

START_TEST(parse_class_with_multiple_method)
  {
    const char content[] = "class Empty {\n  public method_3() {}\n}";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_int_eq(classToken.childrenSize, 1);

    TSParserToken childToken;
    TSFunctionData *methodData;

    childToken = classToken.children[0];
    ck_assert(childToken.tokenType == TS_CLASS_METHOD);
    ck_assert(childToken.visibility == TS_VISIBILITY_PUBLIC);

    methodData = childToken.data;
    ck_assert_ptr_ne(methodData, NULL);
    ck_assert_str_eq(methodData->name, "method_3");
    ck_assert_ptr_eq(methodData->arguments, NULL);
    ck_assert_int_eq(methodData->argumentsSize, 0);
    ck_assert_ptr_eq(methodData->returnType, NULL);
  }
END_TEST

// mixed

START_TEST(parse_class_with_field_and_method)
  {
    const char content[] = "class Empty {\n  field2;\n  bar() {}\n}";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_int_eq(classToken.childrenSize, 2);

    TSParserToken childToken;

    childToken = classToken.children[0];
    ck_assert(childToken.tokenType == TS_CLASS_FIELD);
    ck_assert(childToken.visibility == TS_VISIBILITY_PRIVATE);

    TSLocalVariableData *childData = childToken.data;
    ck_assert_ptr_ne(childData, NULL);
    ck_assert_str_eq(childData->name, "field2");
    ck_assert_ptr_eq(childData->type, NULL);
    ck_assert_ptr_eq(childData->value, NULL);

    childToken = classToken.children[1];
    ck_assert(childToken.tokenType == TS_CLASS_METHOD);
    ck_assert(childToken.visibility == TS_VISIBILITY_PRIVATE);

    TSFunctionData *methodData = childToken.data;
    ck_assert_ptr_ne(methodData, NULL);
    ck_assert_str_eq(methodData->name, "bar");
    ck_assert_ptr_eq(methodData->arguments, NULL);
    ck_assert_int_eq(methodData->argumentsSize, 0);
    ck_assert_ptr_eq(methodData->returnType, NULL);
  }
END_TEST

void parse_classes_suite(Suite *suite) {
  TCase *tc_class = tcase_create("Class");
  tcase_add_test(tc_class, parsing_empty_class);
  tcase_add_test(tc_class, parse_class_with_undefined_field);
  tcase_add_test(tc_class, parse_class_with_private_field);
  tcase_add_test(tc_class, parse_class_with_protected_field);
  tcase_add_test(tc_class, parse_class_with_public_field);

  tcase_add_test(tc_class, parse_class_with_undefined_method);
  tcase_add_test(tc_class, parse_class_with_private_method);
  tcase_add_test(tc_class, parse_class_with_protected_method);
  tcase_add_test(tc_class, parse_class_with_public_method);

  tcase_add_test(tc_class, parse_class_with_field_and_method);
  suite_add_tcase(suite, tc_class);
}