#include "./operators.h"

static const unsigned short int SEMICOLONS_COUNT = 24;
static const unsigned short int VARIABLES_COUNT = 2;
static const unsigned short int OPERATIONS_COUNT = 22;

START_TEST(parse_valid_operators_advenced)
  TS_experimentalFlag = TS_ENABLE_EXPERIMENTAL;
  TSFile *tsFile = TS_parse_file("./examples/operators/valid.ts");
  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 1);

  TSParserToken *scope = NULL, *variableA = NULL, *variableB = NULL, *op = NULL, *semicolon = NULL;

  scope = tsFile->tokens[0];
  ck_assert_eq_ts_scope(scope->tokenType);
  ck_assert_uint_eq(scope->childrenSize, OPERATIONS_COUNT + VARIABLES_COUNT + SEMICOLONS_COUNT);

  variableA = scope->children[0];
  ck_assert_eq_ts_var(variableA->tokenType);

  semicolon = scope->children[1];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  variableB = scope->children[2];
  ck_assert_eq_ts_var(variableB->tokenType);

  semicolon = scope->children[3];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a + b
  op = scope->children[4];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"+");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[5];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a - b
  op = scope->children[6];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"-");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[7];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a * b
  op = scope->children[8];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"*");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[9];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a / b
  op = scope->children[10];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"/");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[11];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a % b
  op = scope->children[12];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"%");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[13];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a == b
  op = scope->children[14];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"==");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[15];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a === b
  op = scope->children[16];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"===");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[17];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a != b
  op = scope->children[18];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"!=");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[19];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a > b
  op = scope->children[20];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L">");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[21];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a >= b
  op = scope->children[22];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L">=");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[23];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a < b
  op = scope->children[24];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"<");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[25];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a <= b
  op = scope->children[26];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"<=");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[27];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a && b
  op = scope->children[28];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"&&");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[29];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a || b
  op = scope->children[30];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"||");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[31];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a & b
  op = scope->children[32];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"&");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[33];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a | b
  op = scope->children[34];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"|");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[35];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a ^ b
  op = scope->children[36];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"^");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 2);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);
  TS_check_validate_borrow(op->children[TS_OPERATOR_B_INDEX], variableB);

  semicolon = scope->children[37];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // !a
  op = scope->children[38];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"!");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 1);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);

  semicolon = scope->children[39];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a++
  op = scope->children[40];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"++");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 1);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);

  semicolon = scope->children[41];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // a--
  op = scope->children[42];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"--");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 1);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);

  semicolon = scope->children[43];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // ++a
  op = scope->children[44];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"++");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 1);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);

  semicolon = scope->children[45];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // --a
  op = scope->children[46];
  ck_assert_ptr_ne(op->name, NULL);
  ck_assert_wstr_eq(op->name, L"--");
  ck_assert_eq_ts_operator(op->tokenType);
  ck_assert_uint_eq(op->childrenSize, 1);
  TS_check_validate_borrow(op->children[TS_OPERATOR_A_INDEX], variableA);

  semicolon = scope->children[47];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  // done
  TS_free_tsFile(tsFile);
  TS_experimentalFlag = TS_DISABLE_EXPERIMENTAL;
END_TEST

void parse_operators_suite(Suite *suite) {
  TCase *tc_op = tcase_create("operators");
  tcase_add_test(tc_op, parse_valid_operators_advenced);
  suite_add_tcase(suite, tc_op);
}