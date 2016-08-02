#include "./functions.h"

START_TEST(parse_empty_function_without_name)
  const char content[] = "function () {}";
  const TSFile tsFile = build_ts_file("memory.ts", content);

  validate_ts_file(tsFile, 1, TS_FUNCTION);

  TSParserToken token = tsFile.tokens[0];
  TSFunctionData *data = token.data;

  ck_assert_ptr_ne(data, NULL);

  ck_assert_ptr_eq(data->name, NULL);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_empty_function_without_name_with_arg)
  const char content[] = "function (arg) {}";
  const TSFile tsFile = build_ts_file("memory.ts", content);

  validate_ts_file(tsFile, 1, TS_FUNCTION);

  TSParserToken token = tsFile.tokens[0];
  TSFunctionData *data = token.data;

  ck_assert_ptr_ne(data, NULL);

  ck_assert_ptr_eq(data->name, NULL);

  ck_assert_int_eq(data->argumentsSize, 1);
  ck_assert_ptr_ne(data->arguments, NULL);

  TSParserToken arg = data->arguments[0];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  TSLocalVariableData *argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "arg");
  ck_assert_ptr_eq(argData->value, NULL);
  ck_assert_ptr_eq(argData->type, NULL);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_empty_function)
  const char content[] = "function example1() {}";
  const TSFile tsFile = build_ts_file("memory.ts", content);

  validate_ts_file(tsFile, 1, TS_FUNCTION);

  TSParserToken token = tsFile.tokens[0];
  TSFunctionData *data = token.data;

  ck_assert_ptr_ne(data, NULL);

  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_str_eq(data->name, "example1");

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_function_with_arg)

  const char content[] = "function example2(first) {}";
  const TSFile tsFile = build_ts_file("memory.ts", content);

  validate_ts_file(tsFile, 1, TS_FUNCTION);

  TSParserToken token = tsFile.tokens[0];
  TSFunctionData *data = token.data;

  ck_assert_ptr_ne(data, NULL);

  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_str_eq(data->name, "example2");

  ck_assert_int_eq(data->argumentsSize, 1);
  ck_assert_ptr_ne(data->arguments, NULL);

  TSParserToken arg = data->arguments[0];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  TSLocalVariableData *argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "first");
  ck_assert_ptr_eq(argData->value, NULL);
  ck_assert_ptr_eq(argData->type, NULL);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_function_with_arg_with_type)
  const char content[] = "function example3(first: number) {}";
  const TSFile tsFile = build_ts_file("memory.ts", content);

  validate_ts_file(tsFile, 1, TS_FUNCTION);

  TSParserToken token = tsFile.tokens[0];
  TSFunctionData *data = token.data;

  ck_assert_ptr_ne(data, NULL);

  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_str_eq(data->name, "example3");

  ck_assert_int_eq(data->argumentsSize, 1);
  ck_assert_ptr_ne(data->arguments, NULL);

  TSParserToken arg = data->arguments[0];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  TSLocalVariableData *argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "first");
  ck_assert_ptr_ne(argData->type, NULL);
  ck_assert_str_eq(argData->type, "number");
  ck_assert_ptr_eq(argData->value, NULL);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_function_with_arg_with_value)
  const char content[] = "function example4(first=1) {}";
  const TSFile tsFile = build_ts_file("memory.ts", content);

  validate_ts_file(tsFile, 1, TS_FUNCTION);

  TSParserToken token = tsFile.tokens[0];
  TSFunctionData *data = token.data;

  ck_assert_ptr_ne(data, NULL);

  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_str_eq(data->name, "example4");

  ck_assert_int_eq(data->argumentsSize, 1);
  ck_assert_ptr_ne(data->arguments, NULL);

  TSParserToken arg = data->arguments[0];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  TSLocalVariableData *argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "first");
  ck_assert_ptr_ne(argData->value, NULL);
  ck_assert_str_eq(argData->value, "1");
  ck_assert_ptr_eq(argData->type, NULL);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_function_with_arg_with_value_and_type)
  const char content[] = "function example5(first: number=234) {}";
  const TSFile tsFile = build_ts_file("memory.ts", content);

  validate_ts_file(tsFile, 1, TS_FUNCTION);

  TSParserToken token = tsFile.tokens[0];
  TSFunctionData *data = token.data;

  ck_assert_ptr_ne(data, NULL);
  ck_assert_str_eq(data->name, "example5");

  ck_assert_int_eq(data->argumentsSize, 1);
  ck_assert_ptr_ne(data->arguments, NULL);

  TSParserToken arg = data->arguments[0];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  TSLocalVariableData *argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "first");
  ck_assert_ptr_ne(argData->type, NULL);
  ck_assert_str_eq(argData->type, "number");
  ck_assert_ptr_ne(argData->value, NULL);
  ck_assert_str_eq(argData->value, "234");

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_function_with_args)
  const char content[] = "function example6(first, second) {}";
  const TSFile tsFile = build_ts_file("memory.ts", content);

  validate_ts_file(tsFile, 1, TS_FUNCTION);

  TSParserToken token = tsFile.tokens[0];
  TSFunctionData *data = token.data;

  ck_assert_ptr_ne(data, NULL);

  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_str_eq(data->name, "example6");

  ck_assert_int_eq(data->argumentsSize, 2);
  ck_assert_ptr_ne(data->arguments, NULL);

  // first
  TSParserToken arg = data->arguments[0];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  TSLocalVariableData *argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "first");
  ck_assert_ptr_eq(argData->type, NULL);
  ck_assert_ptr_eq(argData->value, NULL);

  // second

  arg = data->arguments[1];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "second");
  ck_assert_ptr_eq(argData->type, NULL);
  ck_assert_ptr_eq(argData->value, NULL);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_function_with_args_with_values)
  const char content[] = "function example7(first=1, second=2) {}";
  const TSFile tsFile = build_ts_file("memory.ts", content);

  validate_ts_file(tsFile, 1, TS_FUNCTION);

  TSParserToken token = tsFile.tokens[0];
  TSFunctionData *data = token.data;

  ck_assert_ptr_ne(data, NULL);

  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_str_eq(data->name, "example7");

  ck_assert_int_eq(data->argumentsSize, 2);
  ck_assert_ptr_ne(data->arguments, NULL);

  // first
  TSParserToken arg = data->arguments[0];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  TSLocalVariableData *argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "first");
  ck_assert_ptr_eq(argData->type, NULL);
  ck_assert_ptr_ne(argData->value, NULL);
  ck_assert_str_eq(argData->value, "1");

  // second

  arg = data->arguments[1];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "second");
  ck_assert_ptr_eq(argData->type, NULL);
  ck_assert_ptr_ne(argData->value, NULL);
  ck_assert_str_eq(argData->value, "2");

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_function_with_args_with_types)
  const char content[] = "function example8(first:number, second:Object) {}";
  const TSFile tsFile = build_ts_file("memory.ts", content);

  validate_ts_file(tsFile, 1, TS_FUNCTION);

  TSParserToken token = tsFile.tokens[0];
  TSFunctionData *data = token.data;

  ck_assert_ptr_ne(data, NULL);

  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_str_eq(data->name, "example8");

  ck_assert_int_eq(data->argumentsSize, 2);
  ck_assert_ptr_ne(data->arguments, NULL);

  // first
  TSParserToken arg = data->arguments[0];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  TSLocalVariableData *argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "first");
  ck_assert_ptr_ne(argData->type, NULL);
  ck_assert_str_eq(argData->type, "number");
  ck_assert_ptr_eq(argData->value, NULL);

  // second

  arg = data->arguments[1];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "second");
  ck_assert_ptr_ne(argData->type, NULL);
  ck_assert_str_eq(argData->type, "Object");
  ck_assert_ptr_eq(argData->value, NULL);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_function_with_args_with_types_and_values)
  const char content[] = "function example9(first:number  =234, second:Object=new Object) {}";
  const TSFile tsFile = build_ts_file("memory.ts", content);

  validate_ts_file(tsFile, 1, TS_FUNCTION);

  TSParserToken token = tsFile.tokens[0];
  TSFunctionData *data = token.data;

  ck_assert_ptr_ne(data, NULL);

  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_str_eq(data->name, "example9");

  ck_assert_int_eq(data->argumentsSize, 2);
  ck_assert_ptr_ne(data->arguments, NULL);

  // first
  TSParserToken arg = data->arguments[0];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  TSLocalVariableData *argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "first");
  ck_assert_ptr_ne(argData->type, NULL);
  ck_assert_str_eq(argData->type, "number");
  ck_assert_ptr_ne(argData->value, NULL);
  ck_assert_str_eq(argData->value, "234");

  // second

  arg = data->arguments[1];

  ck_assert(arg.tokenType == TS_VAR);
  ck_assert_ptr_ne(arg.data, NULL);

  argData = arg.data;

  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_str_eq(argData->name, "second");
  ck_assert_ptr_ne(argData->type, NULL);
  ck_assert_str_eq(argData->type, "Object");
  ck_assert_ptr_ne(argData->value, NULL);
  ck_assert_str_eq(argData->value, "new Object");

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_function_with_return_type)
  TSFile tsFile = build_ts_file("memory.ts","function example12():number{}");

  ck_assert_int_eq(tsFile.tokensSize, 1);

  TSParserToken token = tsFile.tokens[0];
  ck_assert(token.tokenType == TS_FUNCTION);

  TSFunctionData *data = token.data;
  ck_assert_ptr_ne(data, NULL);

  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_str_eq(data->name, "example12");

  ck_assert_ptr_ne(data->returnType, NULL);
  ck_assert_str_eq(data->returnType, "number");
END_TEST

START_TEST(parse_function_with_invalid_return_type)
  build_ts_file("memory.ts","function example12():fixed number{}");
END_TEST

START_TEST(parse_function_with_invalid_return_type_name)
  build_ts_file("memory.ts","function example12():fixed%number{}");
END_TEST

START_TEST(parse_function_with_invalid_arg_without_name)
  build_ts_file("memory.ts","function example12(:number) {}");
END_TEST

START_TEST(parse_function_with_invalid_arg_skipping_to_next_before_giving_arg_name)
  build_ts_file("memory.ts","function example12(,b) {}");
END_TEST

START_TEST(parse_function_with_invalid_arg_with_missing_type)
  build_ts_file("memory.ts","function example12(a:) {}");
END_TEST

START_TEST(parse_function_with_invalid_arg_with_missing_value)
  build_ts_file("memory.ts","function example12(a=) {}");
END_TEST

void parse_functions_suite(Suite *suite) {
  TCase *tc_functions = tcase_create("Functions");

  tcase_add_test(tc_functions, parse_empty_function_without_name);
  tcase_add_test(tc_functions, parse_empty_function_without_name_with_arg);

  tcase_add_test(tc_functions, parse_empty_function);
  tcase_add_test(tc_functions, parse_function_with_arg);
  tcase_add_test(tc_functions, parse_function_with_arg_with_value);
  tcase_add_test(tc_functions, parse_function_with_arg_with_type);
  tcase_add_test(tc_functions, parse_function_with_arg_with_value_and_type);
  tcase_add_test(tc_functions, parse_function_with_args);
  tcase_add_test(tc_functions, parse_function_with_args_with_values);
  tcase_add_test(tc_functions, parse_function_with_args_with_types);
  tcase_add_test(tc_functions, parse_function_with_args_with_types_and_values);

  tcase_add_test(tc_functions, parse_function_with_return_type);

  tcase_add_exit_test(tc_functions, parse_function_with_invalid_return_type, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_functions, parse_function_with_invalid_return_type_name, TS_PARSE_FAILURE_CODE);

  tcase_add_exit_test(tc_functions, parse_function_with_invalid_arg_without_name, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_functions, parse_function_with_invalid_arg_skipping_to_next_before_giving_arg_name, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_functions, parse_function_with_invalid_arg_with_missing_type, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_functions, parse_function_with_invalid_arg_with_missing_value, TS_PARSE_FAILURE_CODE);

  suite_add_tcase(suite, tc_functions);
}