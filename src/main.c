#include <tsc/parser.h>
#include <tsc/output.h>

int main(int argc, const char **argv) {
  setlocale(LC_ALL, "");

  FILE *g = fopen("./examples/unicode.ts", "r");
  int invalid = 0;
  while (!feof(g)) {
    int c = fgetc(g);
    if (c == WEOF) break;

    if (c > 127) {
      puts("unicode");
      u_long k = c;

      int count = 1;
      char *byte = calloc(sizeof(char), count);
      byte[0] = c;
      while (!feof(g)) {
        c = fgetc(g);
        if (c <= 128) {
          ungetc(c, g);
          break;
        }
        char *tmp = calloc(sizeof(char), count + 1);
        memcpy(tmp, byte, count);
        tmp[count] = c;
        free(byte);
        byte = tmp;
        count == 1;
        k += c;
      }

      printf("\n# char: '%c' %u \n", byte, k);
      wprintf((wchar_t *) L"## uni: \n");
    }

    printf("char: '%c' %u \n", c, c);
    if (invalid++ > 200) break;
  }

  fclose(g);

  return 0;

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
