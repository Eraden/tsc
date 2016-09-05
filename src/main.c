#include <tsc/parser.h>
#include <tsc/register.h>

int main(int argc, const char **argv) {
  setlocale(LC_ALL, "");

  TSParserSettings settings = TS_parse_arguments(argc, argv);

  init_log();

  const TSFile *tsFile = TS_parse_stream(settings.fileName, settings.stream);

  TS_free_tsFile(tsFile);

  TS_destroy_register();

  if (tsFile->sanity == TS_FILE_VALID)
    return 0;
  else if (tsFile->sanity == TS_FILE_SYNTAX_ERROR)
    return TS_PARSE_FAILURE_CODE;
  else
    return 1;
}
