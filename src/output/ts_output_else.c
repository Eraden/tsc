#include <tsc/output.h>

// PRINT

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-else")))
ts_print_for_else_body(
    const TSFile *tsFile,
    const TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
  for (u_long bodyIndex = 0; bodyIndex < (*tsParserToken).childrenSize; bodyIndex++) {
    TSParserToken bodyToken = (*tsParserToken).children[bodyIndex];
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    TS_print_for_token(tsFile, bodyToken, settings);
  }
}

void
__attribute__(( section("output-else")))
TS_print_for_else(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  const u_long indent = outputSettings.indent;

  TS_print_indent(outputSettings.stream, indent);

  fprintf(outputSettings.stream, "%s", "else {\n");
  fflush(outputSettings.stream);

  ts_print_for_else_body(tsFile, &tsParserToken, outputSettings);

  TS_print_indent(outputSettings.stream, indent);

  fprintf(outputSettings.stream, "%s", "}\n");
}

// STRING

static const char *
__attribute(( visibility("hidden")))
__attribute__(( section("output-else")))
ts_string_for_else_body(
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
    u_long size = TS_STRING_END + strlen(childString);
    if (ifBody != NULL) {
      size += strlen(ifBody) + strlen(childString);
    }
    char *newPointer = (char *) calloc(sizeof(char), size);
    if (ifBody != NULL) strcpy(newPointer, ifBody);
    strcat(newPointer, childString);
    if (ifBody != NULL) free(ifBody);
    ifBody = newPointer;
  }
  return ifBody;
}

const char *
__attribute__(( section("output-else")))
TS_string_for_else(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  const u_long indent = outputSettings.indent;

  char *string = (char *) calloc(sizeof(char), TS_STRING_END + sizeof("else {\n") + (indent * 2));
  for (u_long indentIndex = 0; indentIndex < indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "else {\n");

  {
    const char *elseBody = ts_string_for_else_body(tsFile, &tsParserToken, outputSettings);
    char *newPointer = (char *) calloc(sizeof(char), TS_STRING_END + strlen(string) + strlen(elseBody));
    strcpy(newPointer, string);
    strcat(newPointer, elseBody);
    free((void *) string);
    free((void *) elseBody);
    string = newPointer;
  }

  {
    char *newPointer = (char *) calloc(sizeof(char), TS_STRING_END + strlen(string) + (indent * 2) + strlen("}\n"));
    strcpy(newPointer, string);
    for (u_long indentIndex = 0; indentIndex < indent; indentIndex++) strcat(newPointer, "  ");
    strcat(newPointer, "}\n");
    free(string);
    string = newPointer;
  }

  return string;
}