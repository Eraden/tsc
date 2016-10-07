#include <cts/register.h>
#include "parse_function.h"

START_TEST(parse_valid_functions)
  TSFile *tsFile = TS_parse_file("./examples/functions/valid.ts");

  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 10);

  TSParserToken *token, *arg;
  TSFunctionData *data;

  TSParserToken *ANY = TS_find_class((const wchar_t *) L"", (const wchar_t *) L"any");
  TSParserToken *NUMBER = TS_find_class((const wchar_t *) L"", (const wchar_t *) L"number");
  TSParserToken *OBJECT = TS_find_class((const wchar_t *) L"", (const wchar_t *) L"Object");
  TSParserToken *STRING = TS_find_class((const wchar_t *) L"", (const wchar_t *) L"string");

  // example1
  token = tsFile->tokens[0];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example1");
  ck_assert_ptr_eq(data->returnType, NULL);

  // example2
  token = tsFile->tokens[1];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example2");
  ck_assert_ptr_eq(data->returnType, NULL);
  arg = token->children[0];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_int_eq(arg->childrenSize, 1);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first1");
   ck_assert_ts_token_eq(arg->children[TS_VARIABLE_TYPE], ANY);

  // example3
  token = tsFile->tokens[2];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example3");
  ck_assert_ptr_eq(data->returnType, NULL);
  arg = token->children[0];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first2");
  ck_assert_ts_token_eq(arg->children[TS_VARIABLE_TYPE], ANY);
  ck_assert_wstr_eq(arg->children[TS_VARIABLE_VALUE]->content, L"1");

  // example4
  token = tsFile->tokens[3];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example4");
  ck_assert_ptr_eq(data->returnType, NULL);
  arg = token->children[0];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_int_eq(arg->childrenSize, 1);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first3");
  ck_assert_ts_token_eq(arg->children[TS_VARIABLE_TYPE], NUMBER);

  // example5
  token = tsFile->tokens[4];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 1);
  ck_assert_ptr_ne(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example5");
  ck_assert_ptr_eq(data->returnType, NULL);
  arg = token->children[0];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first4");
  ck_assert_ts_token_eq(arg->children[TS_VARIABLE_TYPE], NUMBER);
  ck_assert_wstr_eq(arg->children[TS_VARIABLE_VALUE]->content, L"234");

  // example6
  token = tsFile->tokens[5];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example6");
  ck_assert_ptr_eq(data->returnType, NULL);
  arg = token->children[0];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_int_eq(arg->childrenSize, 1);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first5");
  // ck_assert_ptr_eq(argData->type, NULL);
//  ck_assert_ptr_eq(argData->value, NULL);
  arg = token->children[1];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_int_eq(arg->childrenSize, 1);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"second5");
  // ck_assert_ptr_eq(argData->type, NULL);
//  ck_assert_ptr_eq(argData->value, NULL);

  token = tsFile->tokens[6];
  data = token->functionData;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example7");
  ck_assert_ptr_eq(data->returnType, NULL);
  arg = token->children[0];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first6");
  // ck_assert_ptr_eq(argData->type, NULL);
//  ck_assert_ptr_ne(argData->value, NULL);
//  ck_assert_wstr_eq(argData->value, L"1");
  arg = token->children[1];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"second6");
  // ck_assert_ptr_eq(argData->type, NULL);
//  ck_assert_ptr_ne(argData->value, NULL);
//  ck_assert_wstr_eq(argData->value, L"2");

  // example8
  token = tsFile->tokens[7];
  data = token->data;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example8");
  ck_assert_ptr_eq(data->returnType, NULL);
  arg = token->children[0];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_int_eq(arg->childrenSize, 1);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first7");
//  ck_assert_ptr_ne(argData->type, NULL);
//  ck_assert_wstr_eq(argData->type, L"number");
//  ck_assert_ptr_eq(argData->value, NULL);
  arg = token->children[1];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_int_eq(arg->childrenSize, 1);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"second7");
//  ck_assert_ptr_ne(argData->type, NULL);
//  ck_assert_wstr_eq(argData->type, L"Object");
//  ck_assert_ptr_eq(argData->value, NULL);

  // example9
  token = tsFile->tokens[8];
  data = token->data;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 2);
  ck_assert_ptr_ne(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example9");
  ck_assert_ptr_eq(data->returnType, NULL);
  arg = token->children[0];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"first8");
  ck_assert(arg->children[TS_VARIABLE_TYPE] == NUMBER);
//  ck_assert_ptr_ne(argData->value, NULL);
//  ck_assert_wstr_eq(argData->value, L"234");
  arg = token->children[1];
  ck_assert(arg->tokenType == TS_ARGUMENT);
  ck_assert_int_eq(arg->childrenSize, 2);
  ck_assert_ptr_ne(arg->children, NULL);
  ck_assert_ptr_ne(arg->name, NULL);
  ck_assert_wstr_eq(arg->name, L"second8");
//  ck_assert_ptr_ne(argData->type, NULL);
//  ck_assert_wstr_eq(argData->type, L"Object");
//  ck_assert_ptr_ne(argData->value, NULL);
//  ck_assert_wstr_eq(argData->value, L"new Object");

  // example10
  token = tsFile->tokens[9];
  data = token->data;
  ck_assert(token->tokenType == TS_FUNCTION);
  ck_assert_int_eq(token->childrenSize, 0);
  ck_assert_ptr_eq(token->children, NULL);
  ck_assert_ptr_ne(data, NULL);
  ck_assert_ptr_ne(data->name, NULL);
  ck_assert_wstr_eq(data->name, L"example10");
  ck_assert_ptr_ne(data->returnType, NULL);
  ck_assert_wstr_eq(data->returnType, L"any");

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_function_with_missing_return_type)
  TSFile *file = TS_parse_file("./examples/functions/missing_return_type");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

START_TEST(parse_function_with_invalid_return_type)
  TSFile *file = TS_parse_file("./examples/functions/invalid_return_type");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

START_TEST(parse_function_with_invalid_return_type_name)
  TSFile *file = TS_parse_file("./examples/functions/invalid_return_type_name");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

START_TEST(parse_function_with_invalid_arg_without_name)
  TSFile *file = TS_parse_file("./examples/functions/missing_arg_name");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

START_TEST(parse_function_with_invalid_arg_with_missing_type)
  TSFile *file = TS_parse_file("./examples/functions/missing_arg_type");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

START_TEST(parse_function_with_invalid_arg_with_missing_value)
  TSFile *file = TS_parse_file("./examples/functions/missing_arg_value");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

START_TEST(parse_function_with_invalid_arg_skipping_to_next_before_giving_arg_name)
  TSFile *file = TS_parse_file("./examples/functions/missing_first_arg_name");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

void parse_functions_suite(Suite *suite) {
  TCase *tc_functions = tcase_create("Functions");

  tcase_add_test(tc_functions, parse_valid_functions);

  tcase_add_test(tc_functions, parse_function_with_missing_return_type);
  tcase_add_test(tc_functions, parse_function_with_invalid_return_type);
  tcase_add_test(tc_functions, parse_function_with_invalid_return_type_name);

  tcase_add_test(tc_functions, parse_function_with_invalid_arg_without_name);
  tcase_add_test(tc_functions, parse_function_with_invalid_arg_with_missing_type);
  tcase_add_test(tc_functions, parse_function_with_invalid_arg_with_missing_value);
  tcase_add_test(tc_functions, parse_function_with_invalid_arg_skipping_to_next_before_giving_arg_name);

  suite_add_tcase(suite, tc_functions);
}