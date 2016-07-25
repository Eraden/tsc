#include "./variables.h"

// const

START_TEST(parse_const)
  {
    const char content[] = "const some;";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    validate_ts_file(tsFile, 1, TS_CONST);
    TSParserToken token = tsFile.tokens[0];
    TSLocalVariableData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_str_eq(data->name, "some");
    ck_assert_ptr_eq(data->type, NULL);
    ck_assert_ptr_eq(data->value, NULL);
  }
END_TEST

START_TEST(parse_const_with_value)
  {
    const char content[] = "const some = 1;";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken token = tsFile.tokens[0];

    ck_assert(token.tokenType == TS_CONST);
    TSLocalVariableData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_str_eq(data->name, "some");
    ck_assert_ptr_eq(data->type, NULL);
    ck_assert_str_eq(data->value, "1");
  }
END_TEST

START_TEST(parse_const_with_type)
  {
    const char content[] = "const some: number;";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    validate_ts_file(tsFile, 1, TS_CONST);
    TSParserToken token = tsFile.tokens[0];
    TSLocalVariableData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_ptr_ne(data->name, NULL);
    ck_assert_str_eq(data->name, "some");
    ck_assert_ptr_ne(data->type, NULL);
    ck_assert_str_eq(data->type, "number");
    ck_assert_ptr_eq(data->value, NULL);
  }
END_TEST

START_TEST(parse_const_with_value_and_type)
  {
    const char content[] = "const some: number = 1;";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    validate_ts_file(tsFile, 1, TS_CONST);
    TSParserToken token = tsFile.tokens[0];
    TSLocalVariableData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_ptr_ne(data->name, NULL);
    ck_assert_str_eq(data->name, "some");
    ck_assert_ptr_ne(data->type, NULL);
    ck_assert_str_eq(data->type, "number");
    ck_assert_ptr_ne(data->value, NULL);
    ck_assert_str_eq(data->value, "1");
  }
END_TEST

// let

START_TEST(parse_let)
  {
    const char content[] = "let some;";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    validate_ts_file(tsFile, 1, TS_LET);
    TSParserToken token = tsFile.tokens[0];
    TSLocalVariableData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_str_eq(data->name, "some");
    ck_assert_ptr_eq(data->type, NULL);
    ck_assert_ptr_eq(data->value, NULL);
  }
END_TEST

START_TEST(parse_let_with_value)
  {
    const char content[] = "let some = 1;";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken token = tsFile.tokens[0];

    ck_assert(token.tokenType == TS_LET);
    TSLocalVariableData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_str_eq(data->name, "some");
    ck_assert_ptr_eq(data->type, NULL);
    ck_assert_str_eq(data->value, "1");
  }
END_TEST

START_TEST(parse_let_with_type)
  {
    const char content[] = "let some: number;";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    validate_ts_file(tsFile, 1, TS_LET);
    TSParserToken token = tsFile.tokens[0];
    TSLocalVariableData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_ptr_ne(data->name, NULL);
    ck_assert_str_eq(data->name, "some");
    ck_assert_ptr_ne(data->type, NULL);
    ck_assert_str_eq(data->type, "number");
    ck_assert_ptr_eq(data->value, NULL);
  }
END_TEST

START_TEST(parse_let_with_value_and_type)
  {
    const char content[] = "let some: number = 1;";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    validate_ts_file(tsFile, 1, TS_LET);
    TSParserToken token = tsFile.tokens[0];
    TSLocalVariableData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_ptr_ne(data->name, NULL);
    ck_assert_str_eq(data->name, "some");
    ck_assert_ptr_ne(data->type, NULL);
    ck_assert_str_eq(data->type, "number");
    ck_assert_ptr_ne(data->value, NULL);
    ck_assert_str_eq(data->value, "1");
  }
END_TEST

// var

START_TEST(parse_var)
  {
    const char content[] = "var some;";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    validate_ts_file(tsFile, 1, TS_VAR);
    TSParserToken token = tsFile.tokens[0];
    TSLocalVariableData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_str_eq(data->name, "some");
    ck_assert_ptr_eq(data->type, NULL);
    ck_assert_ptr_eq(data->value, NULL);
  }
END_TEST

START_TEST(parse_var_with_value)
  {
    const char content[] = "var some = 1;";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    ck_assert_int_eq(tsFile.tokensSize, 1);
    TSParserToken token = tsFile.tokens[0];

    ck_assert(token.tokenType == TS_VAR);
    TSLocalVariableData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_str_eq(data->name, "some");
    ck_assert_ptr_eq(data->type, NULL);
    ck_assert_str_eq(data->value, "1");
  }
END_TEST

START_TEST(parse_var_with_type)
  {
    const char content[] = "var some: number;";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    validate_ts_file(tsFile, 1, TS_VAR);
    TSParserToken token = tsFile.tokens[0];
    TSLocalVariableData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_ptr_ne(data->name, NULL);
    ck_assert_str_eq(data->name, "some");
    ck_assert_ptr_ne(data->type, NULL);
    ck_assert_str_eq(data->type, "number");
    ck_assert_ptr_eq(data->value, NULL);
  }
END_TEST

START_TEST(parse_var_with_value_and_type)
  {
    const char content[] = "var some: number = 1;";
    const TSFile tsFile = build_ts_file("memory.ts", content);

    validate_ts_file(tsFile, 1, TS_VAR);
    TSParserToken token = tsFile.tokens[0];
    TSLocalVariableData *data = token.data;

    ck_assert_ptr_ne(data, NULL);
    ck_assert_ptr_ne(data->name, NULL);
    ck_assert_str_eq(data->name, "some");
    ck_assert_ptr_ne(data->type, NULL);
    ck_assert_str_eq(data->type, "number");
    ck_assert_ptr_ne(data->value, NULL);
    ck_assert_str_eq(data->value, "1");
  }
END_TEST

void parse_variables_suite(Suite *suite) {
  TCase *tc_variables = tcase_create("Variables");
  
  tcase_add_test(tc_variables, parse_let);
  tcase_add_test(tc_variables, parse_let_with_value);
  tcase_add_test(tc_variables, parse_let_with_type);
  tcase_add_test(tc_variables, parse_let_with_value_and_type);

  tcase_add_test(tc_variables, parse_var);
  tcase_add_test(tc_variables, parse_var_with_value);
  tcase_add_test(tc_variables, parse_var_with_type);
  tcase_add_test(tc_variables, parse_var_with_value_and_type);

  tcase_add_test(tc_variables, parse_const);
  tcase_add_test(tc_variables, parse_const_with_value);
  tcase_add_test(tc_variables, parse_const_with_type);
  tcase_add_test(tc_variables, parse_const_with_value_and_type);

  suite_add_tcase(suite, tc_variables);
}