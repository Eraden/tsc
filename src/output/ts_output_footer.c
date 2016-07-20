#include <tsc/output.h>

const char *
__attribute__((__malloc__))
TS_output_footer() {
  char *string = NULL;
  u_long size = TS_STRING_END +
                strlen("\n}(modules, window));\n");
  string = (char *) calloc(sizeof(char), size);
  strcat(string, "\n}(modules, window));\n");
  return string;
}