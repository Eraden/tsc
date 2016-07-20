#include <tsc/output.h>

const char *
TS_string_for_token(
    const TSFile *tsFile,
    TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  switch (tsParserToken.tokenType) {
    case TS_VAR:
      return TS_string_from_var(tsFile, tsParserToken, outputSettings);
    case TS_LET:
      return TS_string_from_var(tsFile, tsParserToken, outputSettings);
    case TS_CONST:
      return TS_string_from_var(tsFile, tsParserToken, outputSettings);
    case TS_CLASS: {
      return TS_string_for_class(tsFile, tsParserToken, outputSettings);
    }
    case TS_FUNCTION:
      return TS_string_from_function(tsFile, tsParserToken, outputSettings);
    case TS_ARROW: {
      char *tag = "=>";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_IF: {
      return TS_string_for_if(tsFile, tsParserToken, outputSettings);
    }
    case TS_ELSE: {
      return TS_string_for_else(tsFile, tsParserToken, outputSettings);
    }
    case TS_RETURN: {
      return ts_string_for_return(tsFile, tsParserToken, outputSettings);
    }
    case TS_COMPONENT: {
      char *tag = "component";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_IMPORT: {
      char *tag = "import";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_EXPORT: {
      char *tag = "export";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_DEFAULT: {
      char *tag = "default";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_SCOPE: {
      char *tag = "unnamed scope";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_EXTENDS: {
      char *tag = "extends";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_IMPLEMENTS: {
      char *tag = "implements";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_CLASS_FIELD:
      return NULL;
    case TS_CLASS_METHOD:
      return NULL;
    case TS_UNKNOWN:
      return NULL;
  }
  return NULL;
}

void
TS_print_stream(
    const TSFile *tsFile,
    FILE *stream
) {
  TSOutputSettings outputSettings;
  outputSettings.indent = 0;
  outputSettings.stream = stream;

  const char *header = TS_output_header();
  fprintf(stream, "%s", header);
  free((void *) header);

  const u_long size = tsFile->tokensSize;
  for (u_long i = 0; i < size; i++) {
    const char *str = TS_string_for_token(tsFile, tsFile->tokens[i], outputSettings);
    if (str != NULL) {
      fprintf(stream, "%s\n", str);
      free((void *) str);
    }
  }

  const char *footer = TS_output_footer();
  fprintf(stream, "%s", footer);
  free((void *) footer);
}

const char *
__attribute__((__unused__))
TS_generate_string_from_file(
    const TSFile *tsFile
) {
  const u_long size = tsFile->tokensSize;
  TSOutputSettings outputSettings;
  outputSettings.indent = 0;
  outputSettings.stream = NULL;
  char *string = NULL;

  const char *header = TS_output_header();

  {
    char *newPointer = (char *) calloc(sizeof(char), strlen(header) + 1);
    strcpy(newPointer, header);
    free((void *) header);
  }
  for (u_long i = 0; i < size; i++) {
    const char *s = TS_string_for_token(tsFile, tsFile->tokens[i], outputSettings);
    if (s != NULL) {
      char *newPointer = calloc(sizeof(char), strlen(s) + strlen(s) + 1);
      strcat(newPointer, string);
      free(string);
      string = newPointer;
      strcat(string, s);
      free((void *) s);
    }
  }

  {
    const char *footer = TS_output_footer();
    char *newPointer = (char *) calloc(sizeof(char), strlen(string) + strlen(footer) + 1);
    strcpy(newPointer, string);
    strcat(newPointer, footer);
    free((void *) string);
    free((void *) footer);
    string = newPointer;
  }

  return string;
}