#include <tsc/parser.h>
#include <tsc/output.h>

int main(int argc, const char **argv) {
  setlocale(LC_ALL, "");

  TSParserSettings settings = TS_parse_arguments(argc, argv);

  init_log();

  TSFile tsFile = TS_parse_stream(settings.fileName, settings.stream);

  fclose(settings.stream);

  fprintf(stdout, "\n// File: %s\n\n", tsFile.file);

  if (TS_output_stream != NULL) {
    TS_print_stream(&tsFile, TS_output_stream);
  } else {
    TS_print_stream(&tsFile, stdout);
  }

  TS_free_tsFile(tsFile);

  return 0;
}
