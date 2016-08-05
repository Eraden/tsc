#include "./test.h"

#include "./parser/variables.h"
#include "./parser/classes.h"
#include "./parser/functions.h"
#include "./parser/if_conditions.h"
#include "./parser/else_conditions.h"
#include "./parser/return_keyword.h"
#include "./parser/parse_exports.h"
#include "parser/inline_comment.h"
#include "parser/multiline_comment.h"
#include "parser/new_keyword.h"

Suite *class_suite(void) {
  Suite *suite = suite_create("Parser");

  parse_variables_suite(suite);
  parse_functions_suite(suite);
  parse_if_conditions_suite(suite);
  parse_else_conditions_suite(suite);
  parse_classes_suite(suite);
//  parse_return_keyword_suite(suite);
//  parse_exports_suite(suite);
//  parse_inline_comment_suite(suite);
//  parse_multiline_comment_suite(suite);
//  parse_new_suite(suite);
  return suite;
}


int main(int argc, char **argv) {
  setlocale(LC_ALL, "");

  enum print_output output_type = CK_NORMAL;
  TSVerbosity tscVerbose = TS_VERBOSITY_OFF;
  for (int i = 0; i < argc; i++) {
    const char *v = argv[i];
    if (strcmp(v, "--verbose") == 0) {
      output_type = CK_VERBOSE;
      tscVerbose = TS_VERBOSITY_INFO;
    }
  }

  TS_set_log_level(tscVerbose);
  init_log();

  Suite *s;
  SRunner *sr;
  int number_failed = 0;

  s = class_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, output_type);
  number_failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}