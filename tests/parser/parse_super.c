#include "./parse_super.h"

START_TEST(parse_valid_super)
  TSFile *tsFile = TS_parse_file("./examples/super/input/valid.ts");

  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_valid(tsFile);
  ck_assert_ptr_ne(tsFile->tokens, NULL);
  ck_assert_uint_eq(tsFile->tokensSize, 3);

  TSParserToken **tokens = tsFile->tokens;
  TSParserToken *baseClassToken = NULL;
  TSParserToken *childClassToken = NULL;
  TSParserToken *advancedClassToken = NULL;

  /*
    class Base {
      constructor() {}
      method1() {}
    }
   */
  baseClassToken = tokens[0];

  ck_begin_token(baseClassToken, TS_CLASS, L"Base", 2);
    ck_begin_child(constructor, TS_CLASS_METHOD, L"constructor", 3, baseClassToken)
    ck_end_child(baseClassToken, TRUE);

    ck_begin_child(method1, TS_CLASS_METHOD, L"method1", 3, baseClassToken)
    ck_end_child(baseClassToken, FALSE);
  ck_end_token();

  /*
    class Child extends Base {
      prop;
      constructor(arg) { super(); this.prop = arg; }
      method1() { super.method1(); }
      method2(arg) { this.prop = arg; }
    }
   */
  tokens += 1;
  childClassToken = tokens[0];

  ck_begin_token(childClassToken, TS_CLASS, L"Child", 5)
    ck_begin_child(extends, TS_EXTENDS, NULL, 1, childClassToken)
      ck_begin_child(borrow, TS_BORROW, NULL, 1, extends)
        ck_begin_child(parent, TS_CLASS, L"Base", baseClassToken->childrenSize, borrow)
          ck_assert_ptr_eq(parent, baseClassToken);
        ck_end_child(borrow, FALSE)
      ck_end_child(extends, FALSE)
    ck_end_child(childClassToken, TRUE)

    ck_begin_child(prop, TS_CLASS_FIELD, L"prop", 2, childClassToken)
    ck_end_child(childClassToken, TRUE)

    ck_begin_child(constructor, TS_CLASS_METHOD, L"constructor", 3, childClassToken)
      ck_begin_child(args, TS_CALL_ARGUMENTS, NULL, 1, constructor)
      ck_end_child(constructor, TRUE)

      ck_begin_child(returnType, TS_FUNCTION_RETURN_TYPE, NULL, 1, constructor)
      ck_end_child(constructor, TRUE)

      ck_begin_child(scope, TS_SCOPE, NULL, 7, constructor)
        ck_begin_child(super, TS_SUPER, NULL, 1, scope)
          ck_begin_child(args, TS_CALL_ARGUMENTS, NULL, 0, super)
          ck_end_child(super, FALSE)
        ck_end_child(scope, TRUE)
      ck_end_child(constructor, FALSE)
    ck_end_child(childClassToken, TRUE)

    ck_begin_child(method1, TS_CLASS_METHOD, L"method1", 3, childClassToken)
      ck_begin_child(args, TS_CALL_ARGUMENTS, NULL, 0, method1)
      ck_end_child(method1, TRUE)

      ck_begin_child(returnType, TS_FUNCTION_RETURN_TYPE, NULL, 1, method1)
      ck_end_child(method1, TRUE)

      ck_begin_child(scope, TS_SCOPE, NULL, 2, method1)
        ck_begin_child(super, TS_SUPER, L"method1", 1, scope)
          ck_begin_child(args, TS_CALL_ARGUMENTS, NULL, 0, super)
          ck_end_child(super, FALSE)
        ck_end_child(scope, TRUE)
      ck_end_child(method1, FALSE)
    ck_end_child(childClassToken, TRUE)

    ck_begin_child(method2, TS_CLASS_METHOD, L"method2", 3, childClassToken)
    ck_end_child(childClassToken, FALSE)
  ck_end_token()

  /*
     class Advanced extends Child {
      constructor(arg) { super(arg); }
      method1() { super.method1(); }
      method2(arg) { super.method2(arg); }
    }
   */
  tokens += 1;
  advancedClassToken = tokens[0];
  ck_begin_token(advancedClassToken, TS_CLASS, L"Advanced", 4)

    ck_begin_child(extendsToken, TS_EXTENDS, NULL, 1, advancedClassToken)
      ck_begin_child(extendsBorrowClass, TS_BORROW, NULL, 1, extendsToken)
        ck_begin_child(parentClass, TS_CLASS, L"Child", 5, extendsBorrowClass)
          ck_assert_ptr_eq(parentClass, childClassToken);
        ck_end_child(extendsBorrowClass, FALSE)
      ck_end_child(extendsToken, FALSE)
    ck_end_child(advancedClassToken, TRUE)

    ck_begin_child(constructor, TS_CLASS_METHOD, L"constructor", 3, advancedClassToken)
      ck_begin_child(args, TS_CALL_ARGUMENTS, NULL, 1, constructor)
      ck_end_child(constructor, TRUE)

      ck_begin_child(returnType, TS_FUNCTION_RETURN_TYPE, NULL, 1, constructor)
      ck_end_child(constructor, TRUE)

      ck_begin_child(body, TS_SCOPE, NULL, 2, constructor)
        ck_begin_child(superToken, TS_SUPER, NULL, 1, body)
          ck_begin_child(callArgs, TS_CALL_ARGUMENTS, NULL, 1, superToken)
            ck_begin_child(arg, TS_ARGUMENT, L"arg", 2, callArgs)
            ck_end_child(callArgs, FALSE)
          ck_end_child(superToken, FALSE)
        ck_end_child(body, TRUE)

        ck_begin_child(semicolon, TS_SEMICOLON, L";", 0, body)
        ck_end_child(body, FALSE)
      ck_end_child(constructor, FALSE)
    ck_end_child(advancedClassToken, TRUE)

    ck_begin_child(method1, TS_CLASS_METHOD, L"method1", 3, advancedClassToken)
      ck_begin_child(callArgs, TS_CALL_ARGUMENTS, NULL, 0, method1)
      ck_end_child(method1, TRUE)

      ck_begin_child(returnType, TS_FUNCTION_RETURN_TYPE, NULL, 1, method1)
      ck_end_child(method1, TRUE)

      ck_begin_child(body, TS_SCOPE, NULL, 2, method1)
        ck_begin_child(superToken, TS_SUPER, L"method1", 1, body)
        ck_end_child(body, TRUE)

        ck_begin_child(semicolon, TS_SEMICOLON, L";", 0, body)
        ck_end_child(body, FALSE)
      ck_end_child(method1, FALSE)
    ck_end_child(advancedClassToken, FALSE)
  ck_end_token()

  TS_free_ts_file(tsFile);
END_TEST

void parse_super_suite(Suite *suite) {
  TCase *tc_super = tcase_create("super keyword");

  tcase_add_test(tc_super, parse_valid_super);

  suite_add_tcase(suite, tc_super);
}