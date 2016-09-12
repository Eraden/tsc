#include "./test.h"

#include "parser/parse_variables.h"
#include "./parser/classes.h"
#include "parser/parse_function.h"
#include "parser/parse_if.h"
#include "parser/parse_else.h"
#include "parser/parse_return.h"
#include "./parser/parse_exports.h"
#include "parser/parse_inline_comment.h"
#include "parser/parse_multiline_comment.h"
#include "parser/parse_new.h"
#include "./parser/parse_decorator.h"
#include "./parser/parse_switch.h"

char **only = NULL;
u_long onlySize = 0;

unsigned char hasOnly(char *str) {
  if (!only) return 1;
  for (u_long i = 0; i < onlySize; i++) {
    if (strcmp(only[i], str) == 0) return 1;
  }
  return 0;
}

Suite *class_suite(void) {
  Suite *suite = suite_create("Parser");

  if (hasOnly("variable")) parse_variables_suite(suite);
  if (hasOnly("function")) parse_functions_suite(suite);
  if (hasOnly("if")) parse_if_conditions_suite(suite);
  if (hasOnly("else")) parse_else_conditions_suite(suite);
  if (hasOnly("class")) parse_classes_suite(suite);
  if (hasOnly("comment") || hasOnly("inline_comment")) parse_inline_comment_suite(suite);
  if (hasOnly("comment") || hasOnly("multiline_comment")) parse_multiline_comment_suite(suite);
  if (hasOnly("new")) parse_new_suite(suite);
  if (hasOnly("export")) parse_exports_suite(suite);
  if (hasOnly("return")) parse_return_keyword_suite(suite);
  if (hasOnly("decorator")) parse_decorator_suite(suite);
  if (hasOnly("switch")) parse_switch_suite(suite);
  return suite;
}

int main(int argc, char **argv) {
  setlocale(LC_ALL, "");

  enum print_output output_type = CK_NORMAL;
  enum fork_status should_fork = CK_FORK;
  TSVerbosity tscVerbose = TS_VERBOSITY_OFF;
  for (int i = 0; i < argc; i++) {
    const char *v = argv[i];
    if (strcmp(v, "--no-fork"))
      should_fork = CK_NOFORK;
    else if (strcmp(v, "--verbose") == 0) {
      output_type = CK_VERBOSE;
      tscVerbose = TS_VERBOSITY_INFO;
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

  TS_set_log_level(tscVerbose);
  init_log();

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

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}