#include <tsc/output.h>

const char *
__attribute__(( section("output-var")))
TS_string_from_var(
    const TSFile *__attribute__((__unused__)) tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  const u_long indent = outputSettings.indent;
  const TSLocalVariableData *data = tsParserToken.data;
  if (data->name == NULL) return NULL;

  const u_long size = TS_STRING_END +
                      (indent * 2) +
                      sizeof("var ") +
                      strlen(data->name) +
                      (
                          data->value != NULL ?
                          strlen(" = ") + strlen(data->value) :
                          0
                      ) +
                      sizeof(";\n");

  char *variable = calloc(sizeof(char), size);
  for (u_long i = 0, l = indent; i < l; i++)
    strcat(variable, "  ");
  strcat(variable, "var ");
  strcat(variable, data->name);
  if (data->value) {
    strcat(variable, " = ");
    strcat(variable, data->value);
  }
  strcat(variable, ";\n");
  return variable;
}