#include "parse_implements.h"

START_TEST(parse_valid_implements)
  TSFile *tsFile = TS_parse_file("./examples/implements/valid.ts");
  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 4);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

  TSParserToken *interface1Token = NULL,
      *interface2Token = NULL,
      *classToken = NULL,
      *implements = NULL,
      *definition = NULL;

  interface1Token = tsFile->tokens[0];
  ck_assert_ptr_ne(interface1Token, NULL);
  ck_assert_eq_ts_interface(interface1Token->tokenType);

  classToken = tsFile->tokens[1];
  ck_assert_ptr_ne(classToken, NULL);
  ck_assert_eq_ts_class(classToken->tokenType);
  ck_assert_uint_eq(classToken->childrenSize, 1);
  ck_assert_ptr_ne(classToken->children, NULL);

  implements = classToken->children[0];
  ck_assert_ptr_ne(implements, NULL);
  ck_assert_eq_ts_implements(implements->tokenType);
  ck_assert_uint_eq(implements->childrenSize, 1);
  ck_assert_ptr_ne(implements->children, NULL);
  definition = implements->children[0];
  ck_assert_ptr_eq(definition, interface1Token);

  interface2Token = tsFile->tokens[2];
  ck_assert_ptr_ne(interface2Token, NULL);
  ck_assert_eq_ts_interface(interface2Token->tokenType);

  classToken = tsFile->tokens[3];
  ck_assert_ptr_ne(classToken, NULL);
  ck_assert_eq_ts_class(classToken->tokenType);
  ck_assert_uint_eq(classToken->childrenSize, 2);
  ck_assert_ptr_ne(classToken->children, NULL);

  implements = classToken->children[0];
  ck_assert_ptr_ne(implements, NULL);
  ck_assert_eq_ts_implements(implements->tokenType);
  ck_assert_uint_eq(implements->childrenSize, 1);
  ck_assert_ptr_ne(implements->children, NULL);
  definition = implements->children[0];
  ck_assert_ptr_eq(definition, interface1Token);

  implements = classToken->children[1];
  ck_assert_ptr_ne(implements, NULL);
  ck_assert_eq_ts_implements(implements->tokenType);
  ck_assert_uint_eq(implements->childrenSize, 1);
  ck_assert_ptr_ne(implements->children, NULL);
  definition = implements->children[0];
  ck_assert_ptr_eq(definition, interface2Token);

  TS_free_tsFile(tsFile);
END_TEST

void parse_implements_suite(Suite *suite) {
  TCase *test_case = tcase_create("Parse implements");

  tcase_add_test(test_case, parse_valid_implements);

  suite_add_tcase(suite, test_case);
}