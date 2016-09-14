#include <tsc/sys.h>
#include <tsc/parser.h>

const unsigned int TS_VERSION_MAJOR = 0;
const unsigned int TS_VERSION_MINOR = 0;
const unsigned int TS_VERSION_PATCH = 1;
FILE *TS_output_stream = NULL;

static TSVerbosity __attribute__((visibility("hidden"))) ts_current_log_level = TS_VERBOSITY_ERROR;

static void __attribute__((visibility("hidden"))) TS_info_msg(void);
static void __attribute__((visibility("hidden"))) ts_syntax_error(const wchar_t *msg, const wchar_t *file, const u_long line, const u_long character);

static void
TS_info_msg(void) {
  printf(
      "Native TypeScript Compiler\n"
          "Version: %i.%i.%i\n"
          "Parameters:\n"
          "  -l --level          log level (off, error, debug, log, info)\n"
          "  -h --help           show this help message\n"
          "  -v --version        show compiler version\n"
          "  -f --file           file to parse\n"
          "  -c --code           code to parse\n"
          "  -o --out            output file\n",
      TS_VERSION_MAJOR, TS_VERSION_MINOR, TS_VERSION_PATCH
  );
}

static void
ts_syntax_error(
    const wchar_t *msg,
    const wchar_t *file,
    const u_long line,
    const u_long character
) {
  log_error((wchar_t *) L"Syntax error: %ls\n", msg);
  ts_log_position(file, line, character);
}

void
ts_token_syntax_error(
    const wchar_t *msg,
    TSFile *tsFile,
    const TSParserToken *token
) {
  tsFile->sanity = TS_FILE_SYNTAX_ERROR;
  ts_syntax_error(
      msg,
      tsFile->file,
      token->character,
      token->line
  );

  u_long len = wcslen(msg) + wcslen(tsFile->file) + 60;
  wchar_t *buffer = calloc(sizeof(wchar_t), len);
  swprintf(
      buffer,
      len,
      (const wchar_t *) L"Syntax error: %ls\n      Position: %ls:%lu:%lu [file:line:character]\n",
      msg, tsFile->file, token->line + 1, token->character
  );
  tsFile->errorReason = buffer;
}

void
ts_log_position(
    const wchar_t *file,
    const u_long character,
    const u_long line
) {
  log_error((wchar_t *) L"      Position: %ls:%lu:%lu [file:line:character]\n", file, line + 1, character);
}

void
TS_set_log_level(
    TSVerbosity verbosity
) {
  ts_current_log_level = verbosity;
}

unsigned char
TS_check_log_level(
    TSVerbosity verbosity
) {
  if (ts_current_log_level == TS_VERBOSITY_OFF) return 0;
  return (unsigned char) (ts_current_log_level >= verbosity ? 1 : 0);
}

const TSParserSettings
TS_parse_arguments(
    int argc,
    const char **argv
) {
  const char *arg;
  TSParserSettings settings;
  settings.stream = NULL;
  settings.fileName = NULL;

  for (int i = 0; i < argc; i++) {
    arg = argv[i];
//    printf("program argument: '%s'\n", arg);

    if (strcmp(arg, "-l") == 0 || strcmp(arg, "--level") == 0) {
      arg = argv[++i];
      char *tmp = calloc(sizeof(char), strlen(arg) + TS_STRING_END);
      strcpy(tmp, arg);
      if (tmp == NULL) {
        continue;
      }

      if (strcmp(tmp, "info") == 0) {
        TS_set_log_level(TS_VERBOSITY_INFO);
      } else if (strcmp(tmp, "log") == 0) {
        TS_set_log_level(TS_VERBOSITY_LOG);
      } else if (strcmp(tmp, "debug") == 0) {
        TS_set_log_level(TS_VERBOSITY_DEBUG);
      } else if (strcmp(tmp, "error") == 0) {
        TS_set_log_level(TS_VERBOSITY_ERROR);
      } else if (strcmp(tmp, "off") == 0) {
        TS_set_log_level(TS_VERBOSITY_OFF);
      } else {
        log_error(L"Invalid log level argument: %s\n", arg);
        exit(EXIT_FAILURE);
      }

      free((void *) tmp);
    } else if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
      TS_info_msg();
      exit(EXIT_SUCCESS);
    } else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0) {
      printf(
          "Native TypeScript Compiler\n"
              "Version: %i.%i.%i\n",
          TS_VERSION_MAJOR, TS_VERSION_MINOR, TS_VERSION_PATCH
      );
      exit(EXIT_SUCCESS);
    } else if (strcmp(arg, "-f") == 0 || strcmp(arg, "--file") == 0) {
      if (i + 1 >= argc) {
        log_error(L"Expecting file name but no more arguments found");
        exit(EXIT_FAILURE);
      }

      settings.fileName = argv[++i];
      settings.stream = fopen(settings.fileName, "r");
      if (settings.stream == NULL) {
        io_panic((wchar_t *) L"Couldn't open source code file");
      }
    } else if (strcmp(arg, "-c") == 0 || strcmp(arg, "--code") == 0) {
      if (i + 1 >= argc) {
        log_error(L"Expecting code but no more arguments found");
        exit(EXIT_FAILURE);
      }

      arg = argv[++i];
      settings.stream = fmemopen((void *) arg, strlen(arg), "r");
      settings.fileName = "(code eval)";
    } else if (strcmp(arg, "-o") == 0 || strcmp(arg, "--out") == 0) {
      if (i + 1 >= argc) {
        log_error(L"Expecting file name but no more arguments found");
        exit(EXIT_FAILURE);
      }

      TS_output_stream = fopen(argv[++i], "w");

      if (TS_output_stream == NULL) {
        log_error(L"Could not open output file: '%s'\nError code: %i\n", argv[i], errno);
        exit(errno);
      }
    }
  }


  if (settings.stream == NULL) {
    log_error(L"No code to parse given.\n");
    TS_info_msg();
    exit(EXIT_FAILURE);
  }
  return settings;
}
