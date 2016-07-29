#include <tsc/sys.h>
#include <tsc/parser.h>

const unsigned int TS_VERSION_MAJOR = 0;
const unsigned int TS_VERSION_MINOR = 0;
const unsigned int TS_VERSION_PATCH = 1;
FILE *TS_stream_to_parse = NULL;
FILE *TS_output_stream = NULL;
const char *TS_file_name = NULL;

static TSVerbosity __attribute__((visibility("hidden"))) ts_current_log_level = TS_VERBOSITY_INFO;

static void __attribute__((visibility("hidden"))) TS_info_msg() {
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

static const char *TS_str_to_lower(const char *str) {
  if (str == NULL) return NULL;
  const size_t l = strlen(str);
  if (l == 0) return NULL;

  char *lower = (char *) calloc(sizeof(char), strlen(str) + TS_STRING_END);
  for (unsigned int i = 0; i < l; i++) {
    lower[i] = (char) tolower(str[i]);
  }
  return lower;
}

void ts_token_syntax_error(const char *msg, const TSFile *tsFile, const TSParserToken *token) {
  ts_syntax_error(msg, tsFile->file, token->character, token->line);
}

void ts_syntax_error(const char *msg, const char *file, const u_long line, const u_long character) {
  log_error("Syntax error: %s\n", msg);
  ts_log_position(file, line, character);
  exit(4);
}

void ts_log_position(const char *file, const u_long character, const u_long line) {
  log_error("      Position: %s:%lu:%lu [line:character]\n", file, line + 1, character);
}

void TS_set_log_level(TSVerbosity verbosity) {
  ts_current_log_level = verbosity;
}

unsigned char TS_check_log_level(TSVerbosity verbosity) {
  if (ts_current_log_level == TS_VERBOSITY_OFF) return 0;
  return (unsigned char) (ts_current_log_level >= verbosity ? 1 : 0);
}

void TS_parse_arguments(int argc, const char **argv) {
  const char *arg, *tmp;

  for (unsigned int i = 0; i < argc; i++) {
    arg = argv[i];

    if (strcmp(arg, "-l") == 0 || strcmp(arg, "--level") == 0) {
      arg = argv[++i];
      tmp = TS_str_to_lower(arg);
      if (tmp == NULL) continue;

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
        fprintf(stderr, "Invalid log level argument: %s\n", arg);
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
      if (i+1 >= argc) {
        fprintf(stderr, "Expecting file name but no more arguments found");
        exit(EXIT_FAILURE);
      }

      TS_stream_to_parse = fopen(argv[++i], "r");
      TS_file_name = TS_clone_string(argv[i]);

      if (TS_stream_to_parse == NULL) {
        fprintf(stderr, "Could not open file: '%s'\nError code: %i\n", argv[i], errno);
        exit(errno);
      }
    } else if (strcmp(arg, "-c") == 0 || strcmp(arg, "--code") == 0) {
      if (i+1 >= argc) {
        fprintf(stderr, "Expecting code but no more arguments found");
        exit(EXIT_FAILURE);
      }

      arg = argv[++i];
      TS_stream_to_parse = fmemopen((void *) arg, strlen(arg), "r");
      TS_file_name = "(code eval)";
    } else if (strcmp(arg, "-o") == 0 || strcmp(arg, "--out") == 0) {
      if (i+1 >= argc) {
        fprintf(stderr, "Expecting file name but no more arguments found");
        exit(EXIT_FAILURE);
      }

      TS_output_stream = fopen(argv[++i], "w");
      TS_file_name = TS_clone_string(argv[i]);

      if (TS_output_stream == NULL) {
        fprintf(stderr, "Could not open output file: '%s'\nError code: %i\n", argv[i], errno);
        exit(errno);
      }
    }
  }
  if (TS_stream_to_parse == NULL) {
    fprintf(stderr, "No code to parse given.\n");
    TS_info_msg();
    exit(EXIT_FAILURE);
  }
}
