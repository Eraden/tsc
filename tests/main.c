#include <cts/register.h>
#include "./test.h"

#include "parser/parse_variables.h"
#include "parser/classes.h"
#include "parser/parse_extends.h"
#include "parser/parse_function.h"
#include "parser/parse_if.h"
#include "parser/parse_else.h"
#include "parser/parse_return.h"
#include "parser/parse_exports.h"
#include "parser/parse_inline_comment.h"
#include "parser/parse_multiline_comment.h"
#include "parser/parse_new.h"
#include "parser/parse_decorator.h"
#include "parser/parse_switch.h"
#include "parser/parse_break.h"
#include "parser/for/parse_for_let.h"
#include "parser/for/parse_for_of.h"
#include "parser/for/parse_for_in.h"
#include "parser/parse_json.h"
#include "parser/parse_implements.h"
#include "parser/parse_while.h"
#include "parser/parse_do.h"
#include "parser/parse_super.h"
#include "parser/parse_spread.h"
#include "core/sys.h"
#include "core/operators.h"
#include "core/borrows.h"

static char **only = NULL;
static u_long onlySize = 0;

unsigned char hasOnly(char *str) {
  if (!only) return 1;
  for (u_long i = 0; i < onlySize; i++) {
    if (strcmp(only[i], str) == 0) return 1;
  }
  return 0;
}

Suite *class_suite(void) {
  Suite *suite = suite_create("Parser");
  if (hasOnly("sys")) parse_sys_suite(suite);
  if (hasOnly("break")) parse_break_suite(suite);

  if (hasOnly("comment") || hasOnly("inline_comment")) parse_inline_comment_suite(suite);
  if (hasOnly("comment") || hasOnly("multiline_comment")) parse_multiline_comment_suite(suite);
  if (hasOnly("borrows")) parse_borrows_suite(suite);
  if (hasOnly("json")) parse_json(suite);
  if (hasOnly("implements")) parse_implements_suite(suite);
  if (hasOnly("operators")) parse_operators_suite(suite);
  if (hasOnly("function")) parse_functions_suite(suite);
  if (hasOnly("if")) parse_if_conditions_suite(suite);
  if (hasOnly("else")) parse_else_conditions_suite(suite);
  if (hasOnly("export")) parse_exports_suite(suite);
  if (hasOnly("return")) parse_return_keyword_suite(suite);
  if (hasOnly("decorator")) parse_decorator_suite(suite);

  if (hasOnly("variable")) parse_variables_suite(suite);
  if (hasOnly("class")) parse_classes_suite(suite);
  if (hasOnly("class") || hasOnly("extends")) parse_extends_suite(suite);
  if (hasOnly("new")) parse_new_suite(suite);
  if (hasOnly("switch")) parse_switch_suite(suite);
  if (hasOnly("for") || hasOnly("for-let")) parse_for_let_suite(suite);
  if (hasOnly("for") || hasOnly("for-of")) parse_for_of_suite(suite);
  if (hasOnly("for") || hasOnly("for-in")) parse_for_in_suite(suite);

  if (hasOnly("while")) parse_while_suite(suite);
  if (hasOnly("do")) parse_do_suite(suite);

  if (hasOnly("super")) parse_super_suite(suite);
  if (hasOnly("spread")) parse_spread_suite(suite);
  return suite;
}

int main(int argc, char **argv) {
  setlocale(LC_ALL, "");

  fprintf(stderr, "TSParserToken size %lu\n\n", sizeof(TSParserToken));

  FILE *errorOutput = tmpfile();
  TS_set_error_output(errorOutput);
//  TS_set_error_output(stderr);

  enum print_output output_type = CK_NORMAL;
  enum fork_status should_fork = CK_FORK;
  TSVerbosity ctsVerbose = TS_VERBOSITY_OFF;

  char **opts = argv;
  for (int i = 0; i < argc; i++) {
    const char *v = *opts;
    opts += 1;
    if (strcmp(v, "--no-fork") == 0)
      should_fork = CK_NOFORK;
    else if (strcmp(v, "--verbose") == 0) {
      output_type = CK_VERBOSE;
      ctsVerbose = TS_VERBOSITY_DEBUG;
    }
    else if (strcmp(v, "--only") == 0) {
      while (1) {
        i++;
        if (i == argc) break;
        v = argv[i];
        if (strlen(v) > 2 && v[0] == '-' && v[1] == '-') {
          break;
        } else {
          char **newPointer = calloc(sizeof(char *), onlySize + 1);
          if (only) {
            memcpy(newPointer, only, onlySize);
            free(only);
          }
          only = newPointer;
          char *strClone = (char*) calloc(sizeof(char), strlen(v) + 1);
          strcpy(strClone, v);
          only[onlySize] = strClone;
          onlySize += 1;
        }
      }
    }
  }

  TS_set_log_level(TS_VERBOSITY_DEBUG);
  TS_init_log();
  TS_set_log_level(ctsVerbose);
//  TS_set_log_level(TS_VERBOSITY_INFO);
  TS_setup_predefined();

  ANY = TS_find_type((const wchar_t *) L"", (const wchar_t *) L"any");
  NUMBER = TS_find_type((const wchar_t *) L"", (const wchar_t *) L"number");
  OBJECT = TS_find_type((const wchar_t *) L"", (const wchar_t *) L"Object");
  STRING_RETURN_TYPE = TS_find_type((const wchar_t *) L"", (const wchar_t *) L"string");
  STRING = TS_find_type((const wchar_t *) L"", (const wchar_t *) L"String");
  FUNCTION = TS_find_type((const wchar_t *) L"", (const wchar_t *) L"Function");
  ARRAY = TS_find_type((const wchar_t *) L"", (const wchar_t *) L"Array");

  Suite *s;
  SRunner *sr;
  int number_failed = 0;

  s = class_suite();
  sr = srunner_create(s);
  srunner_set_fork_status(sr, should_fork);

  srunner_run_all(sr, output_type);
  number_failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  if (only != NULL) {
    for (u_long i = 0; i < onlySize; i++) free(only[i]);
    free(only);
  }

  fflush(errorOutput);
  fclose(errorOutput);

  TS_destroy_register();
  TS_remove_predefined();

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}