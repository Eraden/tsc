#include <tsc/options.h>

static void TS_print_settings(void) {
  fprintf(
      stdout,
      "Program settings:\n"
      "  environment: '%s'\n"
      "  output: '%s'\n"
      "  language requirements: '%s'\n",
      ENVIRONMENT,
      OUTPUT_FILE == NULL ? "stdout": OUTPUT_FILE,
      LANGUAGE_QUALITY_REQUIREMENTS == TSLanguageQualityRequirements_Relax ?
      "relax" : LANGUAGE_QUALITY_REQUIREMENTS == TSLanguageQualityRequirements_Strict ?
                "strict" : LANGUAGE_QUALITY_REQUIREMENTS == TSLanguageQualityRequirements_Strong ?
                           "strong" : "unknown"
  );
}

static void TS_printHelpInfo(void) {
  fprintf(
      stdout,
      "Options:\n"
      "  -e ENV                        Compile environment\n"
      "  -o File Path                  Output file\n"
      "  -r [relax|strict|strong]      Set language parser to given env (default is relax)\n"
      "  -h | --help                   Print this help info\n"
      "\n"
      "  --relax                       Set language parser to relax mode\n"
      "  --strict                      Set language parser to strict mode\n"
      "  --strong                      Set language parser to strong mode\n"
  );
}

void TSC_parse_options(int argc, char **argv) {
  char *content;
  enum TSOptionFlag flag = TSOptionFlag_Undefined;
  ENVIRONMENT = "development";
  LANGUAGE_QUALITY_REQUIREMENTS = TSLanguageQualityRequirements_Relax;
  PROGRAM = cloneString(argv[0]);

  for (int i = 1; i < argc; i++) {
    content = argv[i];
    switch (flag) {
      case TSOptionFlag_Help:
      {
        TS_printHelpInfo();
        exit(EXIT_SUCCESS);
      }
      case TSOptionFlag_Environment:
      {
        ENVIRONMENT = cloneString(content);
        flag = TSOptionFlag_Undefined;
        break;
      }
      case TSOptionFlag_LanguageQualityRequirements:
      {
        if (strcmp(content, "relax") == 0) {
          LANGUAGE_QUALITY_REQUIREMENTS = TSLanguageQualityRequirements_Relax;
        } else if (strcmp(content, "strict") == 0) {
          LANGUAGE_QUALITY_REQUIREMENTS = TSLanguageQualityRequirements_Strict;
        } else if (strcmp(content, "strong") == 0) {
          LANGUAGE_QUALITY_REQUIREMENTS = TSLanguageQualityRequirements_Strong;
        } else {
          fprintf(stderr, "Unknown language requirements '%s'\n", content);
          exit(1);
        }
        flag = TSOptionFlag_Undefined;
        break;
      }
      case TSOptionFlag_Output:
      {
        OUTPUT_FILE = cloneString(content);
        FILE *file = fopen(content, "w");
        if (file == NULL) {
          fprintf(stderr, "Can't open file: %s\n", content);
        }
        fclose(file);
        flag = TSOptionFlag_Undefined;
        break;
      }
      default: {
        if (strcmp(content, "-e") == 0) {
          flag = TSOptionFlag_Environment;
        } else if (strcmp(content, "-o") == 0) {
          flag = TSOptionFlag_Output;
        } else if (strcmp(content, "-h") == 0 || strcmp(content, "--help") == 0) {
          flag = TSOptionFlag_Help;
        } else if (strcmp(content, "-r") == 0) {
          flag = TSOptionFlag_LanguageQualityRequirements;
        } else if (strcmp(content, "--relax") == 0) {
          flag = TSOptionFlag_Undefined;
          LANGUAGE_QUALITY_REQUIREMENTS = TSLanguageQualityRequirements_Relax;
        } else if (strcmp(content, "--strict") == 0) {
          flag = TSOptionFlag_Undefined;
          LANGUAGE_QUALITY_REQUIREMENTS = TSLanguageQualityRequirements_Strict;
        } else if (strcmp(content, "--strong") == 0) {
          flag = TSOptionFlag_Undefined;
          LANGUAGE_QUALITY_REQUIREMENTS = TSLanguageQualityRequirements_Strong;
        }
      }
    }
  }
  switch (flag) {
    case TSOptionFlag_LanguageQualityRequirements:
    {
      fprintf(stderr, "Missing language requirements name\n");
      exit(1);
    }
    case TSOptionFlag_Output:
    {
      fprintf(stderr, "Missing output file path\n");
      exit(1);
    }
    case TSOptionFlag_Environment:
    {
      fprintf(stderr, "Missing environment name\n");
      exit(1);
    }
    default:
      break;
  }
  if (strcmp(ENVIRONMENT, "debug") == 0) {
    TS_print_settings();
  }
  fprintf(stdout, "\n\n");
}
