#include <tsc/output.h>

static const char *
__attribute__(( visibility("hidden") ))
__attribute__(( section("output-class") ))
TS_string_for_class_prototype(
    const TSFile *__attribute__(( __unused__ )) tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  const TSClassData *data = tsParserToken.data;
  char *string = NULL;
  u_long size = (outputSettings.indent * 2) +
                strlen(data->name) +
                strlen(".prototype = Object.create(") + 1;
  if (data->parentClass) {
    size += strlen(data->parentClass);
  } else {
    size += strlen("Object");
  }
  size += strlen(".prototype, { constructor: ");
  size += strlen(data->name);
  size += strlen(" });\n");
  string = (char *) calloc(sizeof(char), size);

  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, data->name);
  strcat(string, ".prototype = Object.create(");
  if (data->parentClass) {
    strcat(string, data->parentClass);
  } else {
    strcat(string, "Object");
  }
  strcat(string, ".prototype, { constructor: ");
  strcat(string, data->name);
  strcat(string, " });\n");

  return string;
}

const char *
__attribute__(( section("output-class") ))
TS_string_for_class(
    const TSFile *__attribute__(( __unused__ )) tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  const TSClassData *data = tsParserToken.data;
  if (data == NULL) return NULL;
  char *string = NULL;
  u_long size = 0;
  size = (outputSettings.indent * 4) +
      strlen("/* class */\nfunction ") +
      strlen(data->name) +
      strlen("() {}\n") + 1;
  string = (char *) calloc(sizeof(char), size);
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "/* class */\n");
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "function ");
  strcat(string, data->name);
  strcat(string, "() {}\n");

  {
    const char *protoString = TS_string_for_class_prototype(tsFile, tsParserToken, outputSettings);
    if (protoString) {
      char *newPointer = (char *) calloc(sizeof(char), size + sizeof(protoString) + 1);
      strcpy(newPointer, string);
      strcat(newPointer, protoString);
      free(string);
      free((void *) protoString);
      string = newPointer;
    }
  }

  return string;
}