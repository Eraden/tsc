#include "./test.h"

#include "./parser/variables.h"
#include "./parser/classes.h"
#include "./parser/functions.h"
#include "./parser/if_conditions.h"
#include "./parser/else_conditions.h"

Suite *class_suite(void) {
  Suite *suite = suite_create("Parser");

  parse_variables_suite(suite);
  parse_functions_suite(suite);
  parse_if_conditions_suite(suite);
  parse_else_conditions_suite(suite);
  parse_classes_suite(suite);
  return suite;
}


int main(void) {
  init_log();

//  initFakeFile();
//  fprintf(fakeFile, "%s\n", "Hello world");
//  char c;
//  printf("Fake file buffer: ");
//  int i = 0;
//  while(!feof(fakeFile)) { if (i++ > 100) break; c = (char) fgetc(fakeFile); if (c == 0) break; printf("%c", c); }
//  printf("\n");
//  fclose(fakeFile);

  Suite *s;
  SRunner *sr;
  int number_failed = 0;

  s = class_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}