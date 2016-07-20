#include <tsc/output.h>

static const char *
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_string_for_class_method(
    const TSOutputSettings __attribute__((__weak__)) outputSettings,
    const TSParserToken *child,
    u_long *methodSize
) {
  const TSFunctionData *methodData = (*child).data;
  const u_long methodIndent = outputSettings.indent + 1;
  (*methodSize) = TS_STRING_END +
                  (outputSettings.indent * 2) +
                  sizeof("proto['") +
                  sizeof(methodData->name) +
                  sizeof("'] = {\n") +
                  (methodIndent * 2) +
                  sizeof("value: function () {\n") +
                  (methodIndent * 2) +
                  sizeof("}\n") +
                  (outputSettings.indent * 2) +
                  sizeof("};\n");

  char *methodString = (char *) calloc(sizeof(char), *methodSize);
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(methodString, "  ");

  strcat(methodString, "proto['");
  strcat(methodString, methodData->name);
  strcat(methodString, "'] = {\n");
  for (u_long indentIndex = 0; indentIndex < methodIndent; indentIndex++)
    strcat(methodString, "  ");
  strcat(methodString, "value: function () {\n");
  for (u_long indentIndex = 0; indentIndex < methodIndent; indentIndex++)
    strcat(methodString, "  ");
  strcat(methodString, "}\n");
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(methodString, "  ");
  strcat(methodString, "};\n");
  return methodString;
}

static const char *
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_string_for_class_field(
    const TSOutputSettings __attribute__((__weak__)) outputSettings,
    const TSParserToken *child,
    u_long *fieldSize
) {
  const TSLocalVariableData *fieldData = (*child).data;
  const u_long fieldIndent = outputSettings.indent + 1;
  (*fieldSize) = TS_STRING_END +
                 (outputSettings.indent * 2) +
                 strlen("var SYMBOL_FOR_") +
                 strlen(fieldData->name) +
                 strlen(" = Symbol();\n") +
                 (outputSettings.indent * 2) +
                 strlen("proto['") +
                 strlen(fieldData->name) +
                 strlen("'] = {\n") +
                 (fieldIndent * 2) +
                 strlen("get: function () { return this[SYMBOL_FOR_") +
                 strlen(fieldData->name) +
                 strlen("] || ") +
                 strlen(fieldData->value ? fieldData->value : "null") +
                 strlen("; },\n") +
                 (fieldIndent * 2) +
                 strlen("set: function (value) { return this[SYMBOL_FOR_") +
                 strlen(fieldData->name) +
                 strlen("] = value; }\n") +
                 (outputSettings.indent * 2) +
                 strlen("};\n");

  char *fieldString = (char *) calloc(sizeof(char), *fieldSize);
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(fieldString, "  ");
  strcat(fieldString, "var SYMBOL_FOR_");
  strcat(fieldString, fieldData->name);
  strcat(fieldString, " = Symbol();\n");

  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(fieldString, "  ");
  strcat(fieldString, "proto['");
  strcat(fieldString, fieldData->name);
  strcat(fieldString, "'] = {\n");

  // getter
  for (u_long indentIndex = 0; indentIndex < fieldIndent; indentIndex++)
    strcat(fieldString, "  ");
  strcat(fieldString, "get: function () { return this[SYMBOL_FOR_");
  strcat(fieldString, fieldData->name);
  strcat(fieldString, "] || ");
  strcat(fieldString, fieldData->value ? fieldData->value : "null");
  strcat(fieldString, "; },\n");

  // setter
  for (u_long indentIndex = 0; indentIndex < fieldIndent; indentIndex++)
    strcat(fieldString, "  ");
  strcat(fieldString, "set: function (value) { return this[SYMBOL_FOR_");
  strcat(fieldString, fieldData->name);
  strcat(fieldString, "] = value; }\n");
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(fieldString, "  ");
  strcat(fieldString, "};\n");
  return fieldString;
}

static const char *
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_string_for_class_prototype(
    const TSFile *__attribute__(( __unused__ )) tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  const TSClassData *data = tsParserToken.data;
  TSOutputSettings settings = outputSettings;
  settings.indent += 1;
  char *string = NULL;

  u_long size = TS_STRING_END +
                // begin function
                (outputSettings.indent * 2) +
                strlen("(function (C, P) {\n") +
                (settings.indent * 2) +
                // body
                strlen("var proto = {};\n") +
                (settings.indent * 2) +
                strlen("proto['constructor'] = { value: P };\n") +
                (settings.indent * 2) +
                strlen("C.prototype = Object.create(P.prototype, proto);\n") +
                // end of function
                (outputSettings.indent * 2) +
                strlen("}(") +
                strlen(data->name) +
                strlen(", ") +
                strlen(data->parentClass ? data->parentClass : "Object") +
                strlen("));\n");

  string = (char *) calloc(sizeof(char), size);

  // begin function
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "(function (C, P) {\n");

  // body
  for (u_long indentIndex = 0; indentIndex < settings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "var proto = {};\n");
  for (u_long indentIndex = 0; indentIndex < settings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "proto['constructor'] = { value: P };\n");

  for (u_long childIndex = 0; childIndex < tsParserToken.childrenSize; childIndex++) {
    TSParserToken child = tsParserToken.children[childIndex];
    switch (child.tokenType) {
      case TS_CLASS_FIELD: {
        u_long fieldSize;
        const char *fieldString = TS_string_for_class_field(settings, &child, &fieldSize);

        char *newPointer = (char *) calloc(sizeof(char), size + fieldSize - 1);
        strcpy(newPointer, string);
        strcat(newPointer, fieldString);
        free(string);
        free((void *) fieldString);
        string = newPointer;
        size = size + fieldSize - 1;
        break;
      }
      case TS_CLASS_METHOD: {
        u_long methodSize;
        const char *methodString = TS_string_for_class_method(settings, &child, &methodSize);

        char *newPointer = (char *) calloc(sizeof(char), size + methodSize - 1);
        strcpy(newPointer, string);
        strcat(newPointer, methodString);
        free(string);
        free((void *) methodString);
        string = newPointer;
        size = size + methodSize - TS_STRING_END;
        break;
      }
      default:
        break;
    }
  }

  for (u_long indentIndex = 0; indentIndex < settings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "C.prototype = Object.create(P.prototype, proto);\n");

  // end of function
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "}(");
  strcat(string, data->name);
  strcat(string, ", ");
  strcat(string, data->parentClass ? data->parentClass : "Object");
  strcat(string, "));\n");

  return string;
}

static const char *
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_string_for_class_constructor(
    const TSFile *__attribute__(( __unused__ )) tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  const TSClassData *data = tsParserToken.data;
  const u_long __attribute__((__unused__)) indent = outputSettings.indent + 1;
  char *string = NULL;

  u_long size = 0;
  size = TS_STRING_END +
         (outputSettings.indent * 2 * sizeof(char)) +
         strlen("/* class */\n") +
         (outputSettings.indent * 2 * sizeof(char)) +
         strlen("function ") +
         strlen(data->name) +
         strlen("() {\n") +
         (outputSettings.indent * 2 * sizeof(char)) +
         strlen("}\n");

  string = (char *) calloc(sizeof(char), size);

  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "/* class */\n");
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "function ");
  strcat(string, data->name);
  strcat(string, "() {\n");
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++)
    strcat(string, "  ");
  strcat(string, "}\n");

  return string;
}

const char *
__attribute__(( section("output-class")))
TS_string_for_class(
    const TSFile *__attribute__(( __unused__ )) tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  if (tsParserToken.data == NULL) return NULL;
  char *string = NULL;

  {
    const char *constructorString = TS_string_for_class_constructor(tsFile, tsParserToken, outputSettings);
    if (constructorString != NULL) {
      char *newPointer = (char *) calloc(sizeof(char), TS_STRING_END + strlen(constructorString));
      strcat(newPointer, constructorString);
      free((void *) constructorString);
      string = newPointer;
    }
  }

  {
    const char *prototypeString = TS_string_for_class_prototype(tsFile, tsParserToken, outputSettings);
    if (prototypeString != NULL) {
      char *newPointer = (char *) calloc(sizeof(char), TS_STRING_END + strlen(string) + strlen(prototypeString));
      strcpy(newPointer, string);
      strcat(newPointer, prototypeString);
      free(string);
      free((void *) prototypeString);
      string = newPointer;
    }
  }

  return string;
}