#include <tsc/parser.h>
#include <tsc/output.h>

int main(int argc, const char **argv) {
  TS_parse_arguments(argc, argv);

  init_log();

  TSFile tsFile = TS_parse_file();

  fprintf(stdout, "\n// File: %s\n\n", tsFile.file);

  if (TS_output_stream != NULL) {
    TS_print_stream(&tsFile, TS_output_stream);
  } else {
    TS_print_stream(&tsFile, stdout);
  }
  return 0;
}
