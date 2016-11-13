#include "parse_variables.h"

static const unsigned int SEMICOLONS_COUNT = 13;
static const unsigned int VARIABLES_COUNT = 16;

// const

START_TEST(parse_valid_variables)
  TSFile *tsFile = TS_parse_file("./examples/variables/valid.ts");
  ck_assert_tsFile_valid(tsFile);
  ck_assert_int_eq(tsFile->tokensSize, SEMICOLONS_COUNT + VARIABLES_COUNT);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

  TSParserToken *token = NULL, *semicolon = NULL;

  // var
  token = tsFile->tokens[0];

  ck_assert_eq_ts_var(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"a");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);
  
  semicolon = tsFile->tokens[1];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[2];
  ck_assert_eq_ts_var(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"b");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_NUMBER_TYPE);

  semicolon = tsFile->tokens[3];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[4];
  ck_assert_eq_ts_var(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"c");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);

  semicolon = tsFile->tokens[5];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[6];
  ck_assert_eq_ts_var(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"d");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_NUMBER_TYPE);

  semicolon = tsFile->tokens[7];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // let
  token = tsFile->tokens[8];
  ck_assert_eq_ts_let(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"e");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);

  semicolon = tsFile->tokens[9];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[10];
  ck_assert_eq_ts_let(token->tokenType);
  ck_assert_eq_ts_let(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"f");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_NUMBER_TYPE);

  semicolon = tsFile->tokens[11];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[12];
  ck_assert_eq_ts_let(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"g");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);

  semicolon = tsFile->tokens[13];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[14];
  ck_assert_eq_ts_let(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"h");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_NUMBER_TYPE);

  semicolon = tsFile->tokens[15];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // const
  token = tsFile->tokens[16];
  ck_assert_eq_ts_const(token->tokenType);
  ck_assert_wstr_eq(token->name, L"i");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);

  semicolon = tsFile->tokens[17];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[18];
  ck_assert(token->tokenType == TS_CONST);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"j");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_NUMBER_TYPE);

  semicolon = tsFile->tokens[19];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[20];
  ck_assert_eq_ts_const(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"k");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);

  semicolon = tsFile->tokens[21];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  token = tsFile->tokens[22];
  ck_assert(token->tokenType == TS_CONST);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"l");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_NUMBER_TYPE);

  semicolon = tsFile->tokens[23];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // inline m, n, o, p;

  token = tsFile->tokens[24];
  ck_assert_eq_ts_var(token->tokenType);
  ck_assert_wstr_eq(token->name, L"m");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);

  token = tsFile->tokens[25];
  ck_assert_eq_ts_var(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"n");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_NUMBER_TYPE);

  token = tsFile->tokens[26];
  ck_assert_eq_ts_var(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"o");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);

  token = tsFile->tokens[27];
  ck_assert_eq_ts_var(token->tokenType);
  ck_assert_ptr_ne(token->name, NULL);
  ck_assert_wstr_eq(token->name, L"p");
  TS_check_validate_borrow(token->children[TS_VARIABLE_TYPE_INDEX], TS_STRING_TYPE);

  semicolon = tsFile->tokens[28];
  ck_assert_ptr_ne(semicolon, NULL);
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  TS_free_ts_file(tsFile);
END_TEST

START_TEST(parse_var_no_name)
  TSFile *file = TS_parse_file("./examples/variables/no_name");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_var_type_without_name)
  TSFile *file = TS_parse_file("./examples/variables/missing_type");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_var_value_without_expression)
  TSFile *file = TS_parse_file("./examples/variables/missing_value");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_var_with_reserved_word_as_name)
  TSFile *file = TS_parse_file("./examples/variables/var_name_reserved_word");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_let_with_reserved_word_as_name)
  TSFile *file = TS_parse_file("./examples/variables/let_name_reserved_word");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
END_TEST

START_TEST(parse_const_with_reserved_word_as_name)
  TSFile *file = TS_parse_file("./examples/variables/const_name_reserved_word");
  ck_assert_ptr_ne(file, NULL);
  ck_assert_tsFile_syntax_error(file);
  TS_free_ts_file(file);
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