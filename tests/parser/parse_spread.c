#include "./parse_spread.h"

START_TEST(parse_valid_spread)
  TSFile *tsFile = TS_parse_file("./examples/spread/input/valid.ts");

  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_valid(tsFile);
  ck_assert_ptr_ne(tsFile->tokens, NULL);
  ck_assert_uint_eq(tsFile->tokensSize, 4);

  TSParserToken **tokens = tsFile->tokens;
  TSParserToken *args = NULL, *arg = NULL, *spread = NULL;
  TSParserToken *fn = NULL, *classToken = NULL, *mth = NULL;

  /* function first(...args) {} */
  fn = tokens[0];
  ck_validate_child(fn, TS_FUNCTION, L"first", 3);

  tokens += 1;
  /* function second(firstArgument, ...functionRest) {} */
  fn = tokens[0];

  tokens += 1;
  /* class Example { constructor(...constructorArgs) {} method(methodArgument, ...methodRest) {} } */
  classToken = tokens[0];

  tokens += 1;
  /* class Child extends Example { secondMethod(...secondMethodArgs) { this.method(1, ...secondMethodArgs); } } */
  classToken = tokens[0];

  TS_free_ts_file(tsFile);
END_TEST

void parse_spread_suite(Suite *suite) {
  TCase *tc_super = tcase_create("spread");

  tcase_add_test(tc_super, parse_valid_spread);

  suite_add_tcase(suite, tc_super);
}
