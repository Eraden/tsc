#include <tsc/output.h>

const char *TS_string_for_token(TSParserToken tsParserToken, TSOutputSettings outputSettings) {
  switch (tsParserToken.tokenType) {
    case TS_VAR:
      return TS_string_from_var(tsParserToken, outputSettings);
    case TS_LET:
      return TS_string_from_var(tsParserToken, outputSettings);
    case TS_CONST:
      return TS_string_from_var(tsParserToken, outputSettings);
    case TS_CLASS: {
      char *tag = "class";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_FUNCTION:
      return TS_string_from_function(tsParserToken, outputSettings);
    case TS_ARROW:{
      char *tag = "=>";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_IF:{
      return TS_string_for_if(tsParserToken, outputSettings);
    }
    case TS_ELSE:{
      char *tag = "else";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_RETURN:{
      char *tag = "return";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_COMPONENT:{
      char *tag = "component";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_IMPORT:{
      char *tag = "import";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_EXPORT:{
      char *tag = "export";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_DEFAULT:{
      char *tag = "default";
      char *s = (char *) calloc(sizeof(char), sizeof(tag) + 1);
      strcat(s, tag);
      return s;
    }
    case TS_UNKNOWN:
      return NULL;
  }
  return NULL;
}

void TS_print_file(TSFile *tsFile) {
  TSOutputSettings outputSettings;
  outputSettings.indent = 0;
//  outputSettings.outputType = TS_OUTPUT_TYPE_STDOUT;

  const u_long size = tsFile->tokensSize;
  for (u_long i = 0; i < size; i++) {
    const char *str = TS_string_for_token(tsFile->tokens[i], outputSettings);
    if (str != NULL) {
      fprintf(stdout, "%s\n", str);
      free((void *) str);
    }
  }
}

const char *TS_generate_string_from_file(TSFile *tsFile) {
  const u_long size = tsFile->tokensSize;
  TSOutputSettings outputSettings;
  outputSettings.indent = 0;
//  outputSettings.outputType = TS_OUTPUT_TYPE_STRING;
  char *string = NULL;
  for (u_long i = 0; i < size; i++) {
    const char *s = TS_string_for_token(tsFile->tokens[i], outputSettings);
    if (s != NULL) {
      if (string == NULL) {
        string = calloc(sizeof(char), strlen(s) + 1);
      } else {
        char *newPointer = calloc(sizeof(char), strlen(s) + strlen(s) + 1);
        strcat(newPointer, string);
        free(string);
        string = newPointer;
      }
      string[strlen(string)] = 0;
      strcat(string, s);
      free((void *) s);
    }
  }
  return string;
}