#include "parse_variables.h"

// const

START_TEST(parse_valid_variables)
  TSFile *tsFile = TS_parse_file("./examples/variables/valid.ts");
  ck_assert_tsFile_valid(tsFile);
  ck_assert_int_eq(tsFile->tokensSize, 16);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

  TSParserToken *token;

  // var
  token = tsFile->tokens[0];

  ck_assert_eq_ts_var(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"a");
  ck_assert_ptr_ne(token->children[0], NULL);
//  ck_assert_ptr_eq(data->value, NULL);

  token = tsFile->tokens[1];
  ck_assert_eq_ts_var(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"b");
  // ck_assert_ptr_ne(data->type, NULL);
//  ck_assert_wstr_eq(data->type, L"number");
//  ck_assert_ptr_eq(data->value, NULL);

  token = tsFile->tokens[2];
  ck_assert_eq_ts_var(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"c");
  // ck_assert_ptr_eq(data->type, NULL);
//  ck_assert_ptr_ne(data->value, NULL);
//  ck_assert_wstr_eq(data->value, L"1");

  token = tsFile->tokens[3];
  ck_assert_eq_ts_var(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"d");
  // ck_assert_ptr_ne(data->type, NULL);
//  ck_assert_wstr_eq(data->type, L"number");
//  ck_assert_ptr_ne(data->value, NULL);
//  ck_assert_wstr_eq(data->value, L"2");

  // let
  token = tsFile->tokens[4];
  ck_assert_eq_ts_let(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"e");
  // ck_assert_ptr_eq(data->type, NULL);
//  ck_assert_ptr_eq(data->value, NULL);

  token = tsFile->tokens[5];
  ck_assert(token->tokenType == TS_LET);
  ck_assert_eq_ts_let(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"f");
  // ck_assert_ptr_ne(data->type, NULL);
//  ck_assert_wstr_eq(data->type, L"number");
//  ck_assert_ptr_eq(data->value, NULL);

  token = tsFile->tokens[6];
  ck_assert_eq_ts_let(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"g");
  // ck_assert_ptr_eq(data->type, NULL);
//  ck_assert_ptr_ne(data->value, NULL);
//  ck_assert_wstr_eq(data->value, L"3");

  token = tsFile->tokens[7];
  ck_assert_eq_ts_let(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"h");
  // ck_assert_ptr_ne(data->type, NULL);
//  ck_assert_wstr_eq(data->type, L"number");
//  ck_assert_ptr_ne(data->value, NULL);
//  ck_assert_wstr_eq(data->value, L"4");

  // const
  token = tsFile->tokens[8];
  ck_assert_eq_ts_const(token->tokenType);
  ck_assert_wstr_eq(token->name, L"i");
  // ck_assert_ptr_eq(data->type, NULL);
//  ck_assert_ptr_eq(data->value, NULL);

  token = tsFile->tokens[9];
  ck_assert(token->tokenType == TS_CONST);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"j");
  // ck_assert_ptr_ne(data->type, NULL);
//  ck_assert_wstr_eq(data->type, L"number");
//  ck_assert_ptr_eq(data->value, NULL);

  token = tsFile->tokens[10];
  ck_assert(token->tokenType == TS_CONST);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"k");
  // ck_assert_ptr_eq(data->type, NULL);
//  ck_assert_ptr_ne(data->value, NULL);
//  ck_assert_wstr_eq(data->value, L"5");

  token = tsFile->tokens[11];
  ck_assert(token->tokenType == TS_CONST);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"l");
  // ck_assert_ptr_ne(data->type, NULL);
//  ck_assert_wstr_eq(data->type, L"number");
//  ck_assert_ptr_ne(data->value, NULL);
//  ck_assert_wstr_eq(data->value, L"6");

  TS_free_tsFile(tsFile);
END_TEST

START_TEST(parse_var_no_name)
  TSFile *file = TS_parse_file("./examples/variables/no_name");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

START_TEST(parse_var_type_without_name)
  TSFile *file = TS_parse_file("./examples/variables/missing_type");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

START_TEST(parse_var_value_without_expression)
  TSFile *file = TS_parse_file("./examples/variables/missing_value");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

START_TEST(parse_var_with_reserved_word_as_name)
  TSFile *file = TS_parse_file("./examples/variables/var_name_reserved_word");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

START_TEST(parse_let_with_reserved_word_as_name)
  TSFile *file = TS_parse_file("./examples/variables/let_name_reserved_word");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

START_TEST(parse_const_with_reserved_word_as_name)
  TSFile *file = TS_parse_file("./examples/variables/const_name_reserved_word");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_tsFile(file);
END_TEST

void parse_variables_suite(Suite *suite) {
  TCase *tc_variables = tcase_create("Variables");

  tcase_add_test(tc_variables, parse_valid_variables);

  tcase_add_test(tc_variables, parse_var_no_name);
  tcase_add_test(tc_variables, parse_var_type_without_name);
  tcase_add_test(tc_variables, parse_var_value_without_expression);
  tcase_add_test(tc_variables, parse_var_with_reserved_word_as_name);
  tcase_add_test(tc_variables, parse_let_with_reserved_word_as_name);
  tcase_add_test(tc_variables, parse_const_with_reserved_word_as_name);

  suite_add_tcase(suite, tc_variables);
}