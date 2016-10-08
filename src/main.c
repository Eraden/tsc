#include <cts/parser.h>
#include <cts/register.h>

int main(int argc, const char **argv) {
  setlocale(LC_ALL, "");

  TSParserSettings settings = TS_parse_arguments(argc, argv);

  init_log();
  TS_setup_predefined();
  log_to_file(
      (wchar_t *) L"Parsing file: %s\n",
      settings.fileName
  );

  TSFile *tsFile = TS_parse_stream(settings.fileName, settings.stream);

  TSFileSanity sanity = tsFile->sanity;

  if (tsFile->stream) {
    fclose(tsFile->stream);
    tsFile->stream = NULL;
  }
  TS_free_tsFile(tsFile);

  TS_destroy_register();
  TS_remove_predefined();

  if (sanity == TS_FILE_VALID)
    return 0;
  else if (sanity == TS_FILE_SYNTAX_ERROR)
    return TS_PARSE_FAILURE_CODE;
  else
    return 1;
}
