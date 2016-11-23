#include "./parse_super.h"

START_TEST(parse_valid_super)
  TSFile *tsFile = TS_parse_file("./examples/super/input/valid.ts");

  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_valid(tsFile);
  ck_assert_ptr_ne(tsFile->tokens, NULL);
  ck_assert_uint_eq(tsFile->tokensSize, 3);

  TSParserToken **tokens = tsFile->tokens;
  TSParserToken *mth = NULL, *prop = NULL;
  TSParserToken *super__token = NULL;
  TSParserToken *callArgs = NULL;
  TSParserToken *baseClassToken = NULL;
  TSParserToken *baseClassConstructor = NULL;
  TSParserToken *childClassToken = NULL;
  TSParserToken *childClassConstructor = NULL;
  TSParserToken *childClassConstructorScope = NULL;
  TSParserToken *childClassMethod1 = NULL;
  TSParserToken *childClassMethod1Scope = NULL;
  TSParserToken *advancedClassToken = NULL;
  TSParserToken *advancedClassConstructor = NULL;
  TSParserToken *advancedClassConstructorScope = NULL;
  TSParserToken *advancedClassMethod1 = NULL;
  TSParserToken *advancedClassMethod1Scope = NULL;
  TSParserToken *advancedClassMethod2 = NULL;
  TSParserToken *advancedClassMethod2Scope = NULL;

  /*
    class Base {
      constructor() {}

      method1() {}
    }
   */
  baseClassToken = tokens[0];
  ck_validate_child(baseClassToken, TS_CLASS, L"Base", 2);
  baseClassConstructor = baseClassToken->children[0];
  ck_validate_child(baseClassConstructor, TS_CLASS_METHOD, L"constructor", 3);
  mth = baseClassToken->children[1];
  ck_validate_child(mth, TS_CLASS_METHOD, L"method1", 3);

  /*
    class Child extends Base {
      prop;

      constructor(arg) {
        super();
        this.prop = arg;
      }

      method1() {
        super.method1();
      }

      method2(arg) {
        this.prop = arg;
      }
    }
   */
  tokens += 1;
  childClassToken = tokens[0];
  ck_validate_child(childClassToken, TS_CLASS, L"Child", 5);
  ck_validate_extends(childClassToken, baseClassToken);
  prop = childClassToken->children[1];
  ck_validate_child(prop, TS_CLASS_FIELD, L"prop", 2);
  childClassConstructor = childClassToken->children[2];
  ck_validate_child(childClassConstructor, TS_CLASS_METHOD, L"constructor", 3);
  childClassConstructorScope = childClassConstructor->children[TS_CLASS_METHOD_BODY_INDEX];
  ck_validate_child(childClassConstructorScope, TS_SCOPE, NULL, 7);
  super__token = childClassConstructorScope->children[0];
  ck_validate_child(super__token, TS_SUPER, NULL, 1);
  callArgs = super__token->children[0];
  ck_validate_child(callArgs, TS_CALL_ARGUMENTS, NULL, 0);
  childClassMethod1 = childClassToken->children[3];
  ck_validate_child(childClassMethod1, TS_CLASS_METHOD, L"method1", 3);
  childClassMethod1Scope = childClassMethod1->children[TS_CLASS_METHOD_BODY_INDEX];
  ck_validate_child(childClassMethod1Scope, TS_SCOPE, NULL, 2); // super + semicolon
  super__token = childClassMethod1Scope->children[0];
  ck_validate_child(super__token, TS_SUPER, L"method1", 1);
  callArgs = super__token->children[0];
  ck_validate_child(callArgs, TS_CALL_ARGUMENTS, NULL, 0);

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