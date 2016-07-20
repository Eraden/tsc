#include <tsc/output.h>

static const char *
__attribute(( visibility("hidden") ))
__attribute__(( section("output-if") ))
ts_string_for_if_body(
    const TSFile *tsFile,
    const TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
  char *ifBody = NULL;
  for (u_long bodyIndex = 0; bodyIndex < (*tsParserToken).childrenSize; bodyIndex++) {
    TSParserToken bodyToken = (*tsParserToken).children[bodyIndex];
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    const char *childString = TS_string_for_token(tsFile, bodyToken, settings);
    if (childString == NULL) {
      continue;
    }
    u_long size = 0;
    if (ifBody == NULL) {
      size = strlen(childString) + 1;
    } else {
      size = strlen(ifBody) + strlen(childString) + 1;
    }
    char *newPointer = (char *) calloc(sizeof(char), size);
    if (ifBody != NULL) strcpy(newPointer, ifBody);
    strcat(newPointer, childString);
    if (ifBody != NULL) free(ifBody);
    ifBody = newPointer;
  }
  return ifBody;
}

static const char *
__attribute(( visibility("hidden") ))
__attribute__(( section("output-if") ))
ts_string_for_if_condition(
    const TSFile *__attribute__((__unused__)) tsFile,
    const TSIfData *data
) {
  char *string = NULL;
  for (u_long conditionIndex = 0; conditionIndex < data->conditionsSize; conditionIndex++) {
    const TSParserToken conditionToken = data->conditions[conditionIndex];
    const char *value = (char *) conditionToken.data;

    u_long size = strlen(value) + TS_STRING_END;
    if (string != NULL) size += strlen(string);

    char *newPointer = (char *) calloc(sizeof(char), size);
    if (string) strcpy(newPointer, string);
    strcat(newPointer, value);
    free(string);
    string = newPointer;
  }
  return string;
}

const char *TS_string_for_if(const TSFile *tsFile, const TSParserToken tsParserToken, TSOutputSettings outputSettings) {
  const u_long indent = outputSettings.indent;
  const TSIfData *data = tsParserToken.data;
  if (!data) return NULL;

  char *string = (char *) calloc(sizeof(char), sizeof("if (") + (indent * 2) + 1);
  for (u_long indentIndex = 0; indentIndex < indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "if (");

  {
    const char *conditionsBody = ts_string_for_if_condition(tsFile, data);
    char *newPointer = (char *) calloc(sizeof(char), strlen(string) + strlen(conditionsBody) + 1);
    strcpy(newPointer, string);
    strcat(newPointer, conditionsBody);
    free((void *) string);
    free((void *) conditionsBody);
    string = newPointer;
  }

  {
    char *newPointer = (char *) calloc(sizeof(char), strlen(string) + strlen(") {\n") + 1);
    strcpy(newPointer, string);
    strcat(newPointer, ") {\n");
    free(string);
    string = newPointer;
  }

  {
    const char *ifBody = ts_string_for_if_body(tsFile, &tsParserToken, outputSettings);
    char *newPointer = (char *) calloc(sizeof(char), strlen(string) + strlen(ifBody) + 1);
    strcpy(newPointer, string);
    strcat(newPointer, ifBody);
    free((void *) string);
    free((void *) ifBody);
    string = newPointer;
  }

  {
    char *newPointer = (char *) calloc(sizeof(char), strlen(string) + (indent * 2) + strlen("}\n") + 1);
    strcpy(newPointer, string);
    for (u_long indentIndex = 0; indentIndex < indent; indentIndex++) strcat(newPointer, "  ");
    strcat(newPointer, "}\n");
    free(string);
    string = newPointer;
  }

  return string;
}