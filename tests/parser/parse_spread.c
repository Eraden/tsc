#include "./parse_spread.h"

START_TEST(parse_valid_spread)
  TSFile *tsFile = TS_parse_file("./examples/spread/input/valid.ts");

  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_valid(tsFile);
  ck_assert_ptr_ne(tsFile->tokens, NULL);
  ck_assert_uint_eq(tsFile->tokensSize, 4);

  TSParserToken **tokens = tsFile->tokens;
  TSParserToken *args = NULL, *arg = NULL, *spread = NULL;
  TSParserToken *fn = NULL, *classToken = NULL, *extends = NULL, *mth = NULL;

  /* function first(...args) {} */
  fn = tokens[0];
  ck_validate_child( fn, TS_FUNCTION, L"first", 3 );
  args = fn->children[TS_FUNCTION_CALL_ARGS_INDEX];
  ck_validate_child(args, TS_CALL_ARGUMENTS, NULL, 1);
  spread = args->children[0];
  ck_validate_child(spread, TS_SPREAD, L"args", 2);

  tokens += 1;
  /* function second(firstArgument, ...functionRest) {} */
  fn = tokens[0];
  ck_validate_child( fn, TS_FUNCTION, L"second", 3 );
  args = fn->children[TS_FUNCTION_CALL_ARGS_INDEX];
  ck_validate_child(args, TS_CALL_ARGUMENTS, NULL, 2);
  arg = args->children[0];
  ck_validate_child(arg, TS_ARGUMENT, L"firstArgument", 2);
  spread = args->children[1];
  ck_validate_child(spread, TS_SPREAD, L"functionRest", 2);

  tokens += 1;
  /* class Example { constructor(...constructorArgs) {} method(methodArgument, ...methodRest) {} } */
  classToken = tokens[0];
  ck_validate_child(classToken, TS_CLASS, L"Example", 2);
  mth = classToken->children[0];
  ck_validate_child(mth, TS_CLASS_METHOD, L"constructor", 3);
  args = mth->children[TS_CLASS_METHOD_CALL_ARGS_INDEX];
  ck_validate_child(args, TS_CALL_ARGUMENTS, NULL, 1);
  spread = args->children[0];
  ck_validate_child(spread, TS_SPREAD, L"constructorArgs", 2);
  mth = classToken->children[1];
  ck_validate_child(mth, TS_CLASS_METHOD, L"method", 3);
  args = mth->children[TS_CLASS_METHOD_CALL_ARGS_INDEX];
  ck_validate_child(args, TS_CALL_ARGUMENTS, NULL, 2);
  arg = args->children[0];
  ck_validate_child(arg, TS_ARGUMENT, L"methodArgument", 2);
  spread = args->children[1];
  ck_validate_child(spread, TS_SPREAD, L"methodRest", 2);

  tokens += 1;
  /* class Child extends Example { secondMethod(...secondMethodArgs) { this.method(1, ...secondMethodArgs); } } */
  classToken = tokens[0];
  ck_validate_child(classToken, TS_CLASS, L"Child", 2);
  extends = classToken->children[0];
  ck_validate_child(extends, TS_EXTENDS, NULL, 1);
  mth = classToken->children[1];
  ck_validate_child(mth, TS_CLASS_METHOD, L"secondMethod", 3);
  args = mth->children[TS_CLASS_METHOD_CALL_ARGS_INDEX];
  ck_validate_child(args, TS_CALL_ARGUMENTS, NULL, 1);
  spread = args->children[0];
  ck_validate_child(spread, TS_SPREAD, L"secondMethodArgs", 2);

  TS_free_ts_file(tsFile);
END_TEST

void parse_spread_suite(Suite *suite) {
  TCase *tc_super = tcase_create("spread");

  tcase_add_test(tc_super, parse_valid_spread);

  suite_add_tcase(suite, tc_super);
}
