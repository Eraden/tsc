#include <tsc/output.h>

// PRINT

void
__attribute(( visibility("hidden") ))
__attribute__(( section("output-return") ))
ts_print_for_return_body(
    TSFile *tsFile,
    const TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
  for (u_long childIndex = 0; childIndex < (*tsParserToken).childrenSize; childIndex++) {
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    TSParserToken *bodyToken = tsParserToken->children[childIndex];

    if (bodyToken->tokenType == TS_UNKNOWN) {
      fprintf(settings.stream, "%ls;\n", (wchar_t *) bodyToken->data);
    } else {
      TS_print_for_token(tsFile, bodyToken, settings);
    }
  }
}

void
ts_print_for_return(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "return ");

  ts_print_for_return_body(tsFile, tsParserToken, outputSettings);
}

// STRING

static const wchar_t *
__attribute(( visibility("hidden") ))
__attribute__(( section("output-return") ))
ts_string_for_return_body(
    TSFile *tsFile,
    const TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  wchar_t *returnBody = NULL;
  for (u_long childIndex = 0; childIndex < (*tsParserToken).childrenSize; childIndex++) {
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    TSParserToken *bodyToken = tsParserToken->children[childIndex];
    const wchar_t *body = NULL;

    if (bodyToken->tokenType == TS_UNKNOWN) {
      body = bodyToken->data;
    } else {
      body = TS_string_for_token(tsFile, bodyToken, settings);
    }

    if (body != NULL) {
      u_long size = TS_STRING_END + wcslen(body) + wcslen((wchar_t *) L";\n");
      if (returnBody != NULL) size += wcslen(returnBody);
      wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
      if (returnBody != NULL) wcscpy(newPointer, returnBody);
      wcscat(newPointer, body);
      wcscat(newPointer, (wchar_t *) L";\n");
      free((void *) body);
      free(returnBody);
      returnBody = newPointer;
    }
  }
  return returnBody;
}

const wchar_t *
ts_string_for_return(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  wchar_t *string = calloc(
      sizeof(wchar_t),
      TS_STRING_END + wcslen((const wchar_t *) L"return ") + (2 * outputSettings.indent)
  );
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    wcscat(string, (wchar_t *) L"  ");
  wcscat(string, (wchar_t *) L"return ");

  const wchar_t *returnBody = ts_string_for_return_body(tsFile, tsParserToken, outputSettings);
  if (returnBody) {
    wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), wcslen(string) + wcslen(returnBody) + 1);
    wcscpy(newPointer, string);
    wcscat(newPointer, returnBody);
    free((void *) string);
    free((void *) returnBody);
    string = newPointer;
  }
  return string;
}