#include "./parse_super.h"

START_TEST(parse_valid_super)
  TSFile *tsFile = TS_parse_file("./examples/super/input/valid.ts");

  ck_assert_ptr_ne(tsFile, NULL);
  ck_assert_tsFile_valid(tsFile);
  ck_assert_ptr_ne(tsFile->tokens, NULL);
  ck_assert_uint_eq(tsFile->tokensSize, 3);

  TSParserToken **tokens = tsFile->tokens;
  TSParserToken *superToken = NULL;
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
  ck_assert_ptr_ne(baseClassToken, NULL);
  ck_assert_eq_ts_class(baseClassToken->tokenType);
  ck_assert_ptr_ne(baseClassToken->children, NULL);
  ck_assert_uint_eq(baseClassToken->childrenSize, 2);
  baseClassConstructor = baseClassToken->children[0];
  ck_assert_ptr_ne(baseClassConstructor, NULL);
  ck_assert_eq_ts_class_method(baseClassConstructor->tokenType);
  ck_assert_uint_eq(baseClassConstructor->childrenSize, 3);

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
  ck_assert_ptr_ne(childClassToken, NULL);
  ck_assert_eq_ts_class(childClassToken->tokenType);
  ck_assert_ptr_ne(childClassToken->children, NULL);
  ck_assert_uint_eq(childClassToken->childrenSize, 5);
  childClassConstructor = childClassToken->children[2];
  ck_assert_ptr_ne(childClassConstructor, NULL);
  ck_assert_eq_ts_class_method(childClassConstructor->tokenType);
  ck_assert_uint_eq(childClassConstructor->childrenSize, 3);
  ck_assert_ptr_ne(childClassConstructor->children, NULL);
  childClassConstructorScope = childClassConstructor->children[TS_CLASS_METHOD_BODY_INDEX];
  ck_assert_ptr_ne(childClassConstructorScope, NULL);
  ck_assert_eq_ts_scope(childClassConstructorScope->tokenType);
  ck_assert_ptr_ne(childClassConstructorScope->children, NULL);
  ck_assert_uint_gt(childClassConstructorScope->childrenSize, 1);
  superToken = childClassConstructorScope->children[0];
  ck_assert_ptr_ne(superToken, NULL);
  ck_assert_eq_ts_super(superToken->tokenType);
  ck_assert_ptr_eq(superToken->name, NULL);
  ck_assert_ptr_ne(superToken->children, NULL);
  ck_assert_uint_eq(superToken->childrenSize, 1);
  callArgs = superToken->children[0];
  ck_assert_ptr_ne(callArgs, NULL);
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);
  childClassMethod1 = childClassToken->children[3];
  ck_assert_ptr_ne(childClassMethod1, NULL);
  ck_assert_eq_ts_class_method(childClassMethod1->tokenType);
  ck_assert_ptr_ne(childClassMethod1->children, NULL);
  ck_assert_uint_eq(childClassMethod1->childrenSize, 3);
  childClassMethod1Scope = childClassMethod1->children[TS_CLASS_METHOD_BODY_INDEX];
  ck_assert_ptr_ne(childClassMethod1Scope, NULL);
  ck_assert_eq_ts_scope(childClassMethod1Scope->tokenType);
  ck_assert_ptr_ne(childClassMethod1Scope->children, NULL);
  ck_assert_uint_eq(childClassMethod1Scope->childrenSize, 2); // super + semicolon
  superToken = childClassMethod1Scope->children[0];
  ck_assert_ptr_ne(superToken, NULL);
  ck_assert_eq_ts_super(superToken->tokenType);
  ck_assert_ptr_ne(superToken->name, NULL);
  ck_assert_wstr_eq(superToken->name, L"method1");
  ck_assert_ptr_ne(superToken->children, NULL);
  ck_assert_uint_eq(superToken->childrenSize, 1);
  callArgs = superToken->children[0];
  ck_assert_ptr_ne(callArgs, NULL);
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);

  /*
     class Advanced extends Child {
      constructor(arg) {
        super(arg);
      }

      method1() {
        super.method1();
      }

      method2(arg) {
        super.method2(arg);
      }
    }
   */
  tokens += 1;
  advancedClassToken = tokens[0];
  ck_assert_ptr_ne(advancedClassToken, NULL);
  ck_assert_eq_ts_class(advancedClassToken->tokenType);
  ck_assert_ptr_ne(advancedClassToken->children, NULL);
  ck_assert_uint_eq(advancedClassToken->childrenSize, 4);
  advancedClassConstructor = advancedClassToken->children[1];
  ck_assert_ptr_ne(advancedClassConstructor, NULL);
  ck_assert_eq_ts_class_method(advancedClassConstructor->tokenType);
  ck_assert_uint_eq(advancedClassConstructor->childrenSize, 3);
  advancedClassConstructorScope = advancedClassConstructor->children[TS_CLASS_METHOD_BODY_INDEX];
  ck_assert_ptr_ne(advancedClassConstructorScope, NULL);
  ck_assert_eq_ts_scope(advancedClassConstructorScope->tokenType);
  ck_assert_ptr_ne(advancedClassConstructorScope->children, NULL);
  ck_assert_uint_eq(advancedClassConstructorScope->childrenSize, 2); // super() + semicolon
  superToken = advancedClassConstructorScope->children[0];
  ck_assert_ptr_ne(superToken, NULL);
  ck_assert_eq_ts_super(superToken->tokenType);
  ck_assert_ptr_eq(superToken->name, NULL);
  ck_assert_ptr_ne(superToken->children, NULL);
  ck_assert_uint_eq(superToken->childrenSize, 1);
  callArgs = superToken->children[0];
  ck_assert_ptr_ne(callArgs, NULL);
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);
  advancedClassMethod1 = advancedClassToken->children[2];
  ck_assert_ptr_ne(advancedClassMethod1, NULL);
  ck_assert_eq_ts_class_method(advancedClassMethod1->tokenType);
  ck_assert_ptr_ne(advancedClassMethod1->children, NULL);
  ck_assert_uint_eq(advancedClassMethod1->childrenSize, 3);
  advancedClassMethod1Scope = advancedClassMethod1->children[TS_CLASS_METHOD_BODY_INDEX];
  ck_assert_ptr_ne(advancedClassMethod1Scope, NULL);
  ck_assert_eq_ts_scope(advancedClassMethod1Scope->tokenType);
  ck_assert_ptr_ne(advancedClassMethod1Scope->children, NULL);
  ck_assert_uint_eq(advancedClassMethod1Scope->childrenSize, 2); // super + semicolon
  superToken = advancedClassMethod1Scope->children[0];
  ck_assert_ptr_ne(superToken, NULL);
  ck_assert_eq_ts_super(superToken->tokenType);
  ck_assert_ptr_ne(superToken->name, NULL);
  ck_assert_wstr_eq(superToken->name, L"method1");
  ck_assert_ptr_ne(superToken->children, NULL);
  ck_assert_uint_eq(superToken->childrenSize, 1);
  callArgs = superToken->children[0];
  ck_assert_ptr_ne(callArgs, NULL);
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);
  advancedClassMethod2 = advancedClassToken->children[3];
  ck_assert_ptr_ne(advancedClassMethod2, NULL);
  ck_assert_eq_ts_class_method(advancedClassMethod2->tokenType);
  ck_assert_ptr_ne(advancedClassMethod2->children, NULL);
  ck_assert_uint_eq(advancedClassMethod2->childrenSize, 3);
  advancedClassMethod2Scope = advancedClassMethod2->children[TS_CLASS_METHOD_BODY_INDEX];
  ck_assert_ptr_ne(advancedClassMethod2Scope, NULL);
  ck_assert_eq_ts_scope(advancedClassMethod2Scope->tokenType);
  ck_assert_ptr_ne(advancedClassMethod2Scope->children, NULL);
  ck_assert_uint_eq(advancedClassMethod2Scope->childrenSize, 2); // super + semicolon
  superToken = advancedClassMethod2Scope->children[0];
  ck_assert_ptr_ne(superToken, NULL);
  ck_assert_eq_ts_super(superToken->tokenType);
  ck_assert_ptr_ne(superToken->name, NULL);
  ck_assert_wstr_eq(superToken->name, L"method2");
  ck_assert_ptr_ne(superToken->children, NULL);
  ck_assert_uint_eq(superToken->childrenSize, 1);
  callArgs = superToken->children[0];
  ck_assert_ptr_ne(callArgs, NULL);
  ck_assert_eq_ts_call_arguments(callArgs->tokenType);

  TS_free_ts_file(tsFile);
END_TEST

void parse_super_suite(Suite *suite) {
  TCase *tc_super = tcase_create("super keyword");

  tcase_add_test(tc_super, parse_valid_super);

  suite_add_tcase(suite, tc_super);
}