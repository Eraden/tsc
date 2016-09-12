#include <tsc/output.h>

// PRINT

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-else")))
ts_print_for_else_body(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
  for (u_long bodyIndex = 0; bodyIndex < (*tsParserToken).childrenSize; bodyIndex++) {
    TSParserToken *bodyToken = tsParserToken->children[bodyIndex];
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    TS_print_for_token(tsFile, bodyToken, settings);
  }
}

void
__attribute__(( section("output-else")))
TS_print_for_else(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  const u_long indent = outputSettings.indent;

  TS_print_indent(outputSettings.stream, indent);

  fprintf(outputSettings.stream, "%s", "else {\n");
  fflush(outputSettings.stream);

  ts_print_for_else_body(tsFile, tsParserToken, outputSettings);

  TS_print_indent(outputSettings.stream, indent);

  fprintf(outputSettings.stream, "%s", "}\n");
}

// STRING

static const wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-else")))
ts_string_for_else_body(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
  wchar_t *ifBody = NULL;
  for (u_long bodyIndex = 0; bodyIndex < (*tsParserToken).childrenSize; bodyIndex++) {
    TSParserToken *bodyToken = tsParserToken->children[bodyIndex];
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    const wchar_t *childString = TS_string_for_token(tsFile, bodyToken, settings);
    if (childString == NULL) {
      continue;
    }
    u_long size = TS_STRING_END + wcslen(childString);
    if (ifBody != NULL) {
      size += wcslen(ifBody) + wcslen(childString);
    }
    wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
    if (ifBody != NULL) wcscpy(newPointer, ifBody);
    wcscat(newPointer, childString);
    if (ifBody != NULL) free(ifBody);
    ifBody = newPointer;
  }
  return ifBody;
}

const wchar_t *
__attribute__(( section("output-else")))
TS_string_for_else(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  const u_long indent = outputSettings.indent;

  wchar_t *string = (wchar_t *) calloc(sizeof(wchar_t), TS_STRING_END + sizeof("else {\n") + (indent * 2));
  for (u_long indentIndex = 0; indentIndex < indent; indentIndex++)
    wcscat(string, (wchar_t *) L"  ");
  wcscat(string, (wchar_t *) L"else {\n");

  {
    const wchar_t *elseBody = ts_string_for_else_body(tsFile, tsParserToken, outputSettings);
    wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), TS_STRING_END + wcslen(string) + wcslen(elseBody));
    wcscpy(newPointer, string);
    wcscat(newPointer, elseBody);
    free((void *) string);
    free((void *) elseBody);
    string = newPointer;
  }

  {
    wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), TS_STRING_END + wcslen(string) + (indent * 2) + wcslen((wchar_t *) L"}\n"));
    wcscpy(newPointer, string);
    for (u_long indentIndex = 0; indentIndex < indent; indentIndex++) wcscat(newPointer, (wchar_t *) L"  ");
    wcscat(newPointer, (wchar_t *) L"}\n");
    free(string);
    string = newPointer;
  }

  return string;
}