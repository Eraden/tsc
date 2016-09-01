#include <tsc/output.h>

// PRINT

void
__attribute__(( section("output-var")))
TS_print_from_var(
    const TSFile *__attribute__((__unused__)) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  const u_long indent = outputSettings.indent;
  const TSLocalVariableData *data = tsParserToken->data;
  if (data->name == NULL) return;

  for (u_long i = 0, l = indent; i < l; i++)
    fprintf(outputSettings.stream, "%s", "  ");
  fprintf(outputSettings.stream, "%s", "var ");
  fprintf(outputSettings.stream, "%ls", data->name);
  if (data->value) {
    fprintf(outputSettings.stream, "%s", " = ");
    fprintf(outputSettings.stream, "%ls", data->value);
  }
  fprintf(outputSettings.stream, "%s", ";\n");
}

// STRING

const wchar_t *
__attribute__(( section("output-var")))
TS_string_from_var(
    const TSFile *__attribute__((__unused__)) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  const u_long indent = outputSettings.indent;
  const TSLocalVariableData *data = tsParserToken->data;
  if (data->name == NULL) return NULL;

  const u_long size = TS_STRING_END +
                      (indent * 2) +
                      sizeof("var ") +
                      wcslen(data->name) +
                      (
                          data->value != NULL ?
                          wcslen((wchar_t *) L" = ") + wcslen(data->value) :
                          0
                      ) +
                      sizeof(";\n");

  wchar_t *variable = calloc(sizeof(wchar_t), size);
  for (u_long i = 0, l = indent; i < l; i++)
    wcscat(variable, (wchar_t *) L"  ");
  wcscat(variable, (wchar_t *) L"var ");
  wcscat(variable, data->name);
  if (data->value) {
    wcscat(variable, (wchar_t *) L" = ");
    wcscat(variable, data->value);
  }
  wcscat(variable, (wchar_t *) L";\n");
  return variable;
}