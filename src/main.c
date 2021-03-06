#include <cts/file.h>
#include <cts/output.h>
#include <cts/register.h>

int main(int argc, const char **argv) {
  setlocale(LC_ALL, "");

  TSParserSettings settings = TS_parse_arguments(argc, argv);

  TS_init_log();
  TS_setup_predefined();

  TS_log_to_file((wchar_t *) L"Parsing file: %s\n", settings.fileName);

  TSFile *tsFile = TS_parse_stream(settings.fileName, settings.stream);
  TS_set_output_stream(tsFile, stdout);
  TSFileSanity sanity = tsFile->sanity;

  if (tsFile->input.stream) {
    fclose(tsFile->input.stream);
    tsFile->input.stream = NULL;
  }

  TS_build_output(tsFile);

  TS_free_ts_file(tsFile);

  TS_destroy_register();
  TS_remove_predefined();

  if (sanity == TS_FILE_VALID)
    return 0;
  else if (sanity == TS_FILE_SYNTAX_ERROR)
    return TS_PARSE_FAILURE_CODE;
  else
    return 1;
}
