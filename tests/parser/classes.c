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

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
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

    TS_free_tsFile(tsFile);
  }
END_TEST

START_TEST(parse_class_with_complex_fields)
  {
    TSFile tsFile = TS_parse_file("examples/multiple_fields.ts");

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
  tcase_add_test(tc_class, parse_class_with_complex_fields);

  suite_add_tcase(suite, tc_class);
}