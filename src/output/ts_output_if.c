#include <tsc/output.h>

// PRINT

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-if")))
ts_print_for_if_body(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
  for (u_long bodyIndex = 0; bodyIndex < (*tsParserToken).childrenSize; bodyIndex++) {
    TSParserToken *bodyToken = (*tsParserToken).children[bodyIndex];
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    TS_print_for_token(tsFile, bodyToken, settings);
  }
}

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-if")))
ts_print_for_if_condition(
    TSFile *__attribute__((__unused__)) tsFile,
    const TSIfData *data,
    const TSOutputSettings outputSettings
) {
  for (u_long conditionIndex = 0; conditionIndex < data->conditionsSize; conditionIndex++) {
    TSParserToken *conditionToken = data->conditions[conditionIndex];
    const wchar_t *value = (wchar_t *) conditionToken->data;
    fprintf(outputSettings.stream, "%ls", value);
  }
}

void
TS_print_for_if(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  const u_long indent = outputSettings.indent;
  const TSIfData *data = tsParserToken->data;
  if (!data) return;

  for (u_long indentIndex = 0; indentIndex < indent; indentIndex++)
    fprintf(outputSettings.stream, "%s", "  ");
  fprintf(outputSettings.stream, "%s", "if (");

  ts_print_for_if_condition(tsFile, data, outputSettings);

  fprintf(outputSettings.stream, "%s", ") {\n");

  ts_print_for_if_body(tsFile, tsParserToken, outputSettings);

  for (u_long indentIndex = 0; indentIndex < indent; indentIndex++)
    fprintf(outputSettings.stream, "%s", "  ");
  fprintf(outputSettings.stream, "%s", "}\n");
}

// STRING

static const wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-if")))
ts_string_for_if_body(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
  wchar_t *ifBody = NULL;
  for (u_long bodyIndex = 0; bodyIndex < (*tsParserToken).childrenSize; bodyIndex++) {
    TSParserToken *bodyToken = (*tsParserToken).children[bodyIndex];
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    const wchar_t *childString = TS_string_for_token(tsFile, bodyToken, settings);
    if (childString == NULL) {
      continue;
    }
    u_long size = 0;
    if (ifBody == NULL) {
      size = wcslen(childString) + 1;
    } else {
      size = wcslen(ifBody) + wcslen(childString) + 1;
    }
    wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
    if (ifBody != NULL) wcscpy(newPointer, ifBody);
    wcscat(newPointer, childString);
    if (ifBody != NULL) free(ifBody);
    ifBody = newPointer;
  }
  return ifBody;
}

static const wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-if")))
ts_string_for_if_condition(
    TSFile *__attribute__((__unused__)) tsFile,
    const TSIfData *data
) {
  wchar_t *string = NULL;
  for (u_long conditionIndex = 0; conditionIndex < data->conditionsSize; conditionIndex++) {
    TSParserToken *conditionToken = data->conditions[conditionIndex];
    const wchar_t *value = (wchar_t *) conditionToken->data;

    u_long size = wcslen(value) + TS_STRING_END;
    if (string != NULL) size += wcslen(string);

    wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
    if (string) wcscpy(newPointer, string);
    wcscat(newPointer, value);
    free(string);
    string = newPointer;
  }
  return string;
}

const wchar_t *
TS_string_for_if(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  const u_long indent = outputSettings.indent;
  const TSIfData *data = tsParserToken->data;
  if (!data) return NULL;

  wchar_t *string = (wchar_t *) calloc(sizeof(wchar_t), wcslen((const wchar_t *) L"if (") + (indent * 2) + 1);

  TS_push_indent_string(string, indent);

  wcscat(string, (const wchar_t *) L"if (");

  {
    const wchar_t *conditionsBody = ts_string_for_if_condition(tsFile, data);
    wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), wcslen(string) + wcslen(conditionsBody) + 1);
    wcscpy(newPointer, string);
    wcscat(newPointer, conditionsBody);
    free((void *) string);
    free((void *) conditionsBody);
    string = newPointer;
  }

  {
    wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), wcslen(string) + wcslen((const wchar_t *) L") {\n") + 1);
    wcscpy(newPointer, string);
    wcscat(newPointer, (const wchar_t *) L") {\n");
    free(string);
    string = newPointer;
  }

  {
    const wchar_t *ifBody = ts_string_for_if_body(tsFile, tsParserToken, outputSettings);
    wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), wcslen(string) + wcslen(ifBody) + 1);
    wcscpy(newPointer, string);
    wcscat(newPointer, ifBody);
    free((void *) string);
    free((void *) ifBody);
    string = newPointer;
  }

  {
    wchar_t *newPointer = (wchar_t *) calloc(
        sizeof(wchar_t),
        wcslen(string) + (indent * 2) + wcslen((const wchar_t *) L"}\n") + TS_STRING_END
    );
    wcscpy(newPointer, string);

    TS_push_indent_string(newPointer, indent);

    wcscat(newPointer, (const wchar_t *) L"}\n");
    free(string);
    string = newPointer;
  }

  return string;
}