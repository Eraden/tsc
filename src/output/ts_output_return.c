#include <tsc/output.h>

static const char *
__attribute(( visibility("hidden") ))
__attribute__(( section("output-return") ))
ts_string_for_return_body(
    const TSFile *tsFile,
    const TSParserToken *tsParserToken,
    TSOutputSettings *outputSettings
) {
  char *returnBody = NULL;
  for (u_long childIndex = 0; childIndex < (*tsParserToken).childrenSize; childIndex++) {
    TSOutputSettings settings = (*outputSettings);
    settings.indent += 1;
    TSParserToken bodyToken = (*tsParserToken).children[childIndex];
    const char *body = NULL;

    if (bodyToken.tokenType == TS_UNKNOWN) {
      body = bodyToken.data;
    } else {
      body = ts_string_for_return(tsFile, bodyToken, settings);
    }

    if (body != NULL) {
      u_long size = strlen(body) + 1 + strlen(";\n");
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

const char *ts_string_for_return(const TSFile *tsFile, const TSParserToken tsParserToken, TSOutputSettings outputSettings) {
  char *string = calloc(sizeof(char), sizeof("return ") + (2 * outputSettings.indent) + 1);
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "return ");

  const char *returnBody = ts_string_for_return_body(tsFile, &tsParserToken, &outputSettings);
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