#include <tsc/output.h>

const char *
__attribute__(( section("output-var") ))
TS_string_from_var(
    const TSFile * __attribute__((__unused__)) tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  const u_long indent = outputSettings.indent;
  const TSLocalVariableData *data = tsParserToken.data;
  if (data->name == NULL) return NULL;
  unsigned size = sizeof(char) * (indent * 2);
  size += sizeof("var ");
  size += strlen(data->name);
  if (data->value != NULL) {
    size += strlen(" = ");
    size += strlen(data->value);
  }
  size += 1; // ';'
  char *variable = calloc(sizeof(char), size + 1);
  for (u_long i = 0, l = indent; i < l; i++) strcat(variable, "  ");
  strcat(variable, "var ");
  strcat(variable, data->name);
  if (data->value) {
    strcat(variable, " = ");
    strcat(variable, data->value);
  }
  strcat(variable, ";");
  variable[size] = 0;
  return variable;
}