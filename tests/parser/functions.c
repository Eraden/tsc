#include "./functions.h"

START_TEST(parse_valid_functions)
  TSFile tsFile = TS_parse_file("./examples/functions/valid.ts");

  ck_assert_int_eq(tsFile.tokensSize, 10);

  TSParserToken *token, *arg;
  TSFunctionData *data;
  TSLocalVariableData *argData;

  // example1
  token = tsFile.tokens[0];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example1");
  ck_assert_ptr_eq(data->returnType, NULL);
  ck_assert_int_eq(data->argumentsSize, 0);
  ck_assert_ptr_eq(data->arguments, NULL);

  // example2
  token = tsFile.tokens[1];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example2");
  ck_assert_ptr_eq(data->returnType, NULL);
  ck_assert_int_eq(data->argumentsSize, 1);
  ck_assert_ptr_ne(data->arguments, NULL);
  arg = data->arguments[0];
  argData = arg->data;
  ck_assert(arg->tokenType == TS_VAR);
  ck_assert_int_eq(arg->childrenSize, 0);
  ck_assert_ptr_eq(arg->children, NULL);
  ck_assert_ptr_ne(argData, NULL);
  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_wstr_eq(argData->name, L"first");
  ck_assert_ptr_eq(argData->type, NULL);
  ck_assert_ptr_eq(argData->value, NULL);

  // example3
  token = tsFile.tokens[2];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example3");
  ck_assert_ptr_eq(data->returnType, NULL);
  ck_assert_int_eq(data->argumentsSize, 1);
  ck_assert_ptr_ne(data->arguments, NULL);
  arg = data->arguments[0];
  argData = arg->data;
  ck_assert(arg->tokenType == TS_VAR);
  ck_assert_int_eq(arg->childrenSize, 0);
  ck_assert_ptr_eq(arg->children, NULL);
  ck_assert_ptr_ne(argData, NULL);
  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_wstr_eq(argData->name, L"first");
  ck_assert_ptr_eq(argData->type, NULL);
  ck_assert_ptr_ne(argData->value, NULL);
  ck_assert_wstr_eq(argData->value, L"1");

  // example4
  token = tsFile.tokens[3];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example4");
  ck_assert_ptr_eq(data->returnType, NULL);
  ck_assert_int_eq(data->argumentsSize, 1);
  ck_assert_ptr_ne(data->arguments, NULL);
  arg = data->arguments[0];
  argData = arg->data;
  ck_assert(arg->tokenType == TS_VAR);
  ck_assert_int_eq(arg->childrenSize, 0);
  ck_assert_ptr_eq(arg->children, NULL);
  ck_assert_ptr_ne(argData, NULL);
  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_wstr_eq(argData->name, L"first");
  ck_assert_ptr_ne(argData->type, NULL);
  ck_assert_wstr_eq(argData->type, L"number");
  ck_assert_ptr_eq(argData->value, NULL);

  // example5
  token = tsFile.tokens[4];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example5");
  ck_assert_ptr_eq(data->returnType, NULL);
  ck_assert_int_eq(data->argumentsSize, 1);
  ck_assert_ptr_ne(data->arguments, NULL);
  arg = data->arguments[0];
  argData = arg->data;
  ck_assert(arg->tokenType == TS_VAR);
  ck_assert_int_eq(arg->childrenSize, 0);
  ck_assert_ptr_eq(arg->children, NULL);
  ck_assert_ptr_ne(argData, NULL);
  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_wstr_eq(argData->name, L"first");
  ck_assert_ptr_ne(argData->type, NULL);
  ck_assert_wstr_eq(argData->type, L"number");
  ck_assert_ptr_ne(argData->value, NULL);
  ck_assert_wstr_eq(argData->value, L"234");

  // example6
  token = tsFile.tokens[5];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example6");
  ck_assert_ptr_eq(data->returnType, NULL);
  ck_assert_int_eq(data->argumentsSize, 2);
  ck_assert_ptr_ne(data->arguments, NULL);
  arg = data->arguments[0];
  argData = arg->data;
  ck_assert(arg->tokenType == TS_VAR);
  ck_assert_int_eq(arg->childrenSize, 0);
  ck_assert_ptr_eq(arg->children, NULL);
  ck_assert_ptr_ne(argData, NULL);
  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_wstr_eq(argData->name, L"first");
  ck_assert_ptr_eq(argData->type, NULL);
  ck_assert_ptr_eq(argData->value, NULL);
  arg = data->arguments[1];
  argData = arg->data;
  ck_assert(arg->tokenType == TS_VAR);
  ck_assert_int_eq(arg->childrenSize, 0);
  ck_assert_ptr_eq(arg->children, NULL);
  ck_assert_ptr_ne(argData, NULL);
  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_wstr_eq(argData->name, L"second");
  ck_assert_ptr_eq(argData->type, NULL);
  ck_assert_ptr_eq(argData->value, NULL);

  token = tsFile.tokens[6];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example7");
  ck_assert_ptr_eq(data->returnType, NULL);
  ck_assert_int_eq(data->argumentsSize, 2);
  ck_assert_ptr_ne(data->arguments, NULL);
  arg = data->arguments[0];
  argData = arg->data;
  ck_assert(arg->tokenType == TS_VAR);
  ck_assert_int_eq(arg->childrenSize, 0);
  ck_assert_ptr_eq(arg->children, NULL);
  ck_assert_ptr_ne(argData, NULL);
  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_wstr_eq(argData->name, L"first");
  ck_assert_ptr_eq(argData->type, NULL);
  ck_assert_ptr_ne(argData->value, NULL);
  ck_assert_wstr_eq(argData->value, L"1");
  arg = data->arguments[1];
  argData = arg->data;
  ck_assert(arg->tokenType == TS_VAR);
  ck_assert_int_eq(arg->childrenSize, 0);
  ck_assert_ptr_eq(arg->children, NULL);
  ck_assert_ptr_ne(argData, NULL);
  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_wstr_eq(argData->name, L"second");
  ck_assert_ptr_eq(argData->type, NULL);
  ck_assert_ptr_ne(argData->value, NULL);
  ck_assert_wstr_eq(argData->value, L"2");

  // example8
  token = tsFile.tokens[7];
  data = token->data;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example8");
  ck_assert_ptr_eq(data->returnType, NULL);
  ck_assert_int_eq(data->argumentsSize, 2);
  ck_assert_ptr_ne(data->arguments, NULL);
  arg = data->arguments[0];
  argData = arg->data;
  ck_assert(arg->tokenType == TS_VAR);
  ck_assert_int_eq(arg->childrenSize, 0);
  ck_assert_ptr_eq(arg->children, NULL);
  ck_assert_ptr_ne(argData, NULL);
  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_wstr_eq(argData->name, L"first");
  ck_assert_ptr_ne(argData->type, NULL);
  ck_assert_wstr_eq(argData->type, L"number");
  ck_assert_ptr_eq(argData->value, NULL);
  arg = data->arguments[1];
  argData = arg->data;
  ck_assert(arg->tokenType == TS_VAR);
  ck_assert_int_eq(arg->childrenSize, 0);
  ck_assert_ptr_eq(arg->children, NULL);
  ck_assert_ptr_ne(argData, NULL);
  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_wstr_eq(argData->name, L"second");
  ck_assert_ptr_ne(argData->type, NULL);
  ck_assert_wstr_eq(argData->type, L"Object");
  ck_assert_ptr_eq(argData->value, NULL);

  // example9
  token = tsFile.tokens[8];
  data = token->data;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example9");
  ck_assert_ptr_eq(data->returnType, NULL);
  ck_assert_int_eq(data->argumentsSize, 2);
  ck_assert_ptr_ne(data->arguments, NULL);
  arg = data->arguments[0];
  argData = arg->data;
  ck_assert(arg->tokenType == TS_VAR);
  ck_assert_int_eq(arg->childrenSize, 0);
  ck_assert_ptr_eq(arg->children, NULL);
  ck_assert_ptr_ne(argData, NULL);
  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_wstr_eq(argData->name, L"first");
  ck_assert_ptr_ne(argData->type, NULL);
  ck_assert_wstr_eq(argData->type, L"number");
  ck_assert_ptr_ne(argData->value, NULL);
  ck_assert_wstr_eq(argData->value, L"234");
  arg = data->arguments[1];
  argData = arg->data;
  ck_assert(arg->tokenType == TS_VAR);
  ck_assert_int_eq(arg->childrenSize, 0);
  ck_assert_ptr_eq(arg->children, NULL);
  ck_assert_ptr_ne(argData, NULL);
  ck_assert_ptr_ne(argData->name, NULL);
  ck_assert_wstr_eq(argData->name, L"second");
  ck_assert_ptr_ne(argData->type, NULL);
  ck_assert_wstr_eq(argData->type, L"Object");
  ck_assert_ptr_ne(argData->value, NULL);
  ck_assert_wstr_eq(argData->value, L"new Object");

  // example10
  token = tsFile.tokens[9];
  data = token->data;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example10");
  ck_assert_ptr_ne(data->returnType, NULL);
  ck_assert_wstr_eq(data->returnType, L"any");
  ck_assert_int_eq(data->argumentsSize, 0);
  ck_assert_ptr_eq(data->arguments, NULL);

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_function_with_missing_return_type)
  TS_parse_file("./examples/functions/missing_return_type");
END_TEST

START_TEST(parse_function_with_invalid_return_type)
  TS_parse_file("./examples/functions/invalid_return_type");
END_TEST

START_TEST(parse_function_with_invalid_return_type_name)
  TS_parse_file("./examples/functions/invalid_return_type_name");
END_TEST

START_TEST(parse_function_with_invalid_arg_without_name)
  TS_parse_file("./examples/functions/missing_arg_name");
END_TEST

START_TEST(parse_function_with_invalid_arg_with_missing_type)
  TS_parse_file("./examples/functions/missing_arg_type");
END_TEST

START_TEST(parse_function_with_invalid_arg_with_missing_value)
  TS_parse_file("./examples/functions/missing_arg_value");
END_TEST

START_TEST(parse_function_with_invalid_arg_skipping_to_next_before_giving_arg_name)
  TS_parse_file("./examples/functions/missing_first_arg_name");
END_TEST

void parse_functions_suite(Suite *suite) {
  TCase *tc_functions = tcase_create("Functions");

  tcase_add_test(tc_functions, parse_valid_functions);

  tcase_add_exit_test(tc_functions, parse_function_with_missing_return_type, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_functions, parse_function_with_invalid_return_type, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_functions, parse_function_with_invalid_return_type_name, TS_PARSE_FAILURE_CODE);

  tcase_add_exit_test(tc_functions, parse_function_with_invalid_arg_without_name, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_functions, parse_function_with_invalid_arg_with_missing_type, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_functions, parse_function_with_invalid_arg_with_missing_value, TS_PARSE_FAILURE_CODE);
  tcase_add_exit_test(tc_functions, parse_function_with_invalid_arg_skipping_to_next_before_giving_arg_name, TS_PARSE_FAILURE_CODE);

  suite_add_tcase(suite, tc_functions);
}