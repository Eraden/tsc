#include <tsc/output.h>

const char *TS_string_for_if(TSParserToken tsParserToken, TSOutputSettings outputSettings) {
  const u_long indent = outputSettings.indent;
  const TSIfData *data = tsParserToken.data;
  if (!data) return NULL;

  char *string = (char *) calloc(sizeof(char), sizeof("if (") + (indent * 2) + 1);
  for (u_long indentIndex = 0; indentIndex < indent; indentIndex++) strcat(string, "  ");
  strcat(string, "if (");

  for (u_long conditionIndex = 0; conditionIndex < data->conditionsSize; conditionIndex++) {
    const TSParserToken conditionToken = data->conditions[conditionIndex];
    const char *value = (char *) conditionToken.data;

    char *newPointer = (char *) calloc(sizeof(char), strlen(string) + strlen(value) + 1);
    strcpy(newPointer, string);
    strcat(newPointer, value);
    free(string);
    string = newPointer;
  }

  {
    char *newPointer = (char *) calloc(sizeof(char), strlen(string) + strlen(") {\n") + 1);
    strcpy(newPointer, string);
    strcat(newPointer, ") {\n");
    free(string);
    string = newPointer;
  }

  for (u_long bodyIndex = 0; bodyIndex < tsParserToken.childrenSize; bodyIndex++) {
    // TODO body
  }

  {
    char *newPointer = (char *) calloc(sizeof(char), strlen(string) + (indent * 2) + strlen("}") + 1);
    strcpy(newPointer, string);
    for (u_long indentIndex = 0; indentIndex < indent; indentIndex++) strcat(newPointer, "  ");
    strcat(newPointer, "}");
    free(string);
    string = newPointer;
  }

  return string;
}