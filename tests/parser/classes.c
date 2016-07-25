#include "./classes.h"

START_TEST(parsing_empty_class)
  {
    FILE *stream;
    const char fileName[] = "memory.ts";
    const char content[] = "class Empty {}";
    stream = fmemopen((void *) content, strlen(content), "r");
    const TSFile tsFile = TS_parse_stream(fileName, stream);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_str_eq(classData->name, "Empty");
    ck_assert_int_eq(classToken.childrenSize, 0);
  }
END_TEST

START_TEST(class_with_fields)
  {
    FILE *stream;
    const char fileName[] = "memory.ts";
    const char content[] = "class Empty {\n  field;\n}";
    stream = fmemopen((void *) content, strlen(content), "r");
    const TSFile tsFile = TS_parse_stream(fileName, stream);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken classToken = tsFile.tokens[0];

    ck_assert(classToken.tokenType == TS_CLASS);
    TSClassData *classData = classToken.data;

    ck_assert_ptr_ne(classData, NULL);
    ck_assert_int_eq(classToken.childrenSize, 1);

    TSParserToken fieldToken = classToken.children[0];

    ck_assert(fieldToken.tokenType == TS_CLASS_FIELD);
  }
END_TEST

void parse_classes_suite(Suite *suite) {
  TCase *tc_class = tcase_create("Class");
  tcase_add_test(tc_class, parsing_empty_class);
  tcase_add_test(tc_class, class_with_fields);
  suite_add_tcase(suite, tc_class);
}