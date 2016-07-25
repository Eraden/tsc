#include <tsc/output.h>

// PRINT

void
__attribute(( visibility("hidden") ))
__attribute__(( section("output-return") ))
ts_print_for_return_body(
    const TSFile *tsFile,
    const TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
  for (u_long childIndex = 0; childIndex < (*tsParserToken).childrenSize; childIndex++) {
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    TSParserToken bodyToken = (*tsParserToken).children[childIndex];

    if (bodyToken.tokenType == TS_UNKNOWN) {
      fprintf(settings.stream, "%s", (char *) bodyToken.data);
    } else {
      TS_print_for_token(tsFile, bodyToken, settings);
    }
  }
}

void
ts_print_for_return(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    fprintf(outputSettings.stream, "%s", "  ");
  fprintf(outputSettings.stream, "%s", "return ");

  ts_print_for_return_body(tsFile, &tsParserToken, outputSettings);
}

// STRING

static const char *
__attribute(( visibility("hidden") ))
__attribute__(( section("output-return") ))
ts_string_for_return_body(
    const TSFile *tsFile,
    const TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  char *returnBody = NULL;
  for (u_long childIndex = 0; childIndex < (*tsParserToken).childrenSize; childIndex++) {
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    TSParserToken bodyToken = (*tsParserToken).children[childIndex];
    const char *body = NULL;

    if (bodyToken.tokenType == TS_UNKNOWN) {
      body = bodyToken.data;
    } else {
      body = TS_string_for_token(tsFile, bodyToken, settings);
    }

    if (body != NULL) {
      u_long size = TS_STRING_END + strlen(body) + strlen(";\n");
      if (returnBody != NULL) size += strlen(returnBody);
      char *newPointer = (char *) calloc(sizeof(char), size);
      if (returnBody != NULL) strcpy(newPointer, returnBody);
      strcat(newPointer, body);
      strcat(newPointer, ";\n");
      free((void *) body);
      free(returnBody);
      returnBody = newPointer;
    }
  }
  return returnBody;
}

const char *
ts_string_for_return(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  char *string = calloc(sizeof(char), TS_STRING_END + sizeof("return ") + (2 * outputSettings.indent));
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "return ");

  const char *returnBody = ts_string_for_return_body(tsFile, &tsParserToken, outputSettings);
  if (returnBody) {
    char *newPointer = (char *) calloc(sizeof(char), strlen(string) + strlen(returnBody) + 1);
    strcpy(newPointer, string);
    strcat(newPointer, returnBody);
    free((void *) string);
    free((void *) returnBody);
    string = newPointer;
  }
  return string;
}