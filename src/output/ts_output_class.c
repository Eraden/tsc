#include <tsc/output.h>

// PRINT

static void
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_print_for_class_method(
    const TSFile *tsFile,
    const TSOutputSettings __attribute__((__weak__)) outputSettings,
    const TSParserToken *child
) {
  const TSFunctionData *methodData = (*child).data;
  const u_long methodIndent = outputSettings.indent + 1;

  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "proto['");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", methodData->name);
  fprintf(outputSettings.stream, "%s", "'] = {\n");
  fflush(outputSettings.stream);

  TS_print_indent(outputSettings.stream, methodIndent);

  fprintf(outputSettings.stream, "%s", "value: function () {\n");
  fflush(outputSettings.stream);

  TSParserToken body;
  for (u_long bodyIndex = 0; bodyIndex < child->childrenSize; bodyIndex++) {
    body = child->children[bodyIndex];
    TSOutputSettings settings = outputSettings;
    settings.indent = methodIndent + 1;
    TS_print_for_token(tsFile, body, settings);
  }

  TS_print_indent(outputSettings.stream, methodIndent);

  fprintf(outputSettings.stream, "%s", "}\n");
  fflush(outputSettings.stream);

  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "};\n");
  fflush(outputSettings.stream);
}

static void
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_print_for_class_field(
    const TSFile *__attribute__(( __unused__ )) tsFile,
    const TSOutputSettings __attribute__((__weak__)) outputSettings,
    const TSParserToken *child
) {
  const TSLocalVariableData *fieldData = (*child).data;
  const u_long fieldIndent = outputSettings.indent + 1;

  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "var SYMBOL_FOR_");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", fieldData->name);
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", " = Symbol();\n");
  fflush(outputSettings.stream);

  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "proto['");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", fieldData->name);
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", "'] = {\n");
  fflush(outputSettings.stream);

  // getter
  TS_print_indent(outputSettings.stream, fieldIndent);

  fprintf(outputSettings.stream, "%s", "get: function () { return this[SYMBOL_FOR_");
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%s", fieldData->name);
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%s", "] == void(0) ? ");
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%s", fieldData->value ? fieldData->value : "null");
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%s", " : this[SYMBOL_FOR_");
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%s", fieldData->name);
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%s", "]");
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%s", "; },\n");
  fflush(outputSettings.stream);

  // setter
  TS_print_indent(outputSettings.stream, fieldIndent);

  fprintf(outputSettings.stream, "%s", "set: function (value) { return this[SYMBOL_FOR_");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", fieldData->name);
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", "] = value; }\n");
  fflush(outputSettings.stream);

  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "};\n");
  fflush(outputSettings.stream);
}

static void
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_print_for_class_prototype(
    const TSFile *__attribute__(( __unused__ )) tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  const TSClassData *data = tsParserToken.data;
  TSOutputSettings settings = outputSettings;
  settings.indent += 1;

  // begin function
  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++) {
    fprintf(outputSettings.stream, "%s", "  ");
    fflush(outputSettings.stream);
  }
  fprintf(outputSettings.stream, "%s", "(function (C, P) {\n");
  fflush(outputSettings.stream);

  // body
  TS_print_indent(outputSettings.stream, settings.indent);

  fprintf(outputSettings.stream, "%s", "var proto = {};\n");
  fflush(outputSettings.stream);

  TS_print_indent(outputSettings.stream, settings.indent);

  fprintf(outputSettings.stream, "%s", "proto['constructor'] = { value: P };\n");
  fflush(outputSettings.stream);

  for (u_long childIndex = 0; childIndex < tsParserToken.childrenSize; childIndex++) {
    TSParserToken child = tsParserToken.children[childIndex];
    switch (child.tokenType) {
      case TS_CLASS_FIELD: {
        TS_print_for_class_field(tsFile, settings, &child);
        break;
      }
      case TS_CLASS_METHOD: {
        TS_print_for_class_method(tsFile, settings, &child);
        break;
      }
      default:
        break;
    }
  }

  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "C.prototype = Object.create(P.prototype, proto);\n");
  fflush(outputSettings.stream);

  // end of function
  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "}(");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", data->name);
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", ", ");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", data->parentClass ? data->parentClass : "Object");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", "));\n");
  fflush(outputSettings.stream);
}

static void
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_print_for_class_constructor(
    const TSFile *__attribute__(( __unused__ )) tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  const TSClassData *data = tsParserToken.data;
  const u_long __attribute__((__unused__)) indent = outputSettings.indent + 1;

  for (u_long indentIndex = 0; indentIndex < outputSettings.indent; indentIndex++) {
    fprintf(outputSettings.stream, "%s", "  ");
    fflush(outputSettings.stream);
  }
  fprintf(outputSettings.stream, "%s", "/* class */\n");
  fflush(outputSettings.stream);

  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "function ");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", data->name);
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", "() {\n");
  fflush(outputSettings.stream);

  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "}\n");
  fflush(outputSettings.stream);
}

void
__attribute__(( section("output-class")))
TS_print_for_class(
    const TSFile *__attribute__(( __unused__ )) tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  if (tsParserToken.data == NULL) return;
  TS_print_for_class_constructor(tsFile, tsParserToken, outputSettings);
  TS_print_for_class_prototype(tsFile, tsParserToken, outputSettings);
}

// STRING

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

  TS_push_indent_string(methodString, outputSettings.indent);

  strcat(methodString, "proto['");
  strcat(methodString, methodData->name);
  strcat(methodString, "'] = {\n");

  TS_push_indent_string(methodString, methodIndent);

  strcat(methodString, "value: function () {\n");

  TS_push_indent_string(methodString, methodIndent);

  strcat(methodString, "}\n");

  TS_push_indent_string(methodString, outputSettings.indent);

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
                 strlen("] == void(0) ? ") +
                 strlen(fieldData->value ? fieldData->value : "null") +
                 strlen(" : this[SYMBOL_FOR_") +
                 strlen(fieldData->name) +
                 strlen("]") +
                 strlen("; },\n") +
                 (fieldIndent * 2) +
                 strlen("set: function (value) { return this[SYMBOL_FOR_") +
                 strlen(fieldData->name) +
                 strlen("] = value; }\n") +
                 (outputSettings.indent * 2) +
                 strlen("};\n");

  char *fieldString = (char *) calloc(sizeof(char), *fieldSize);

  TS_push_indent_string(fieldString, outputSettings.indent);

  strcat(fieldString, "var SYMBOL_FOR_");
  strcat(fieldString, fieldData->name);
  strcat(fieldString, " = Symbol();\n");

  TS_push_indent_string(fieldString, outputSettings.indent);

  strcat(fieldString, "proto['");
  strcat(fieldString, fieldData->name);
  strcat(fieldString, "'] = {\n");

  // getter
  for (u_long indentIndex = 0; indentIndex < fieldIndent; indentIndex++)
    strcat(fieldString, "  ");
  strcat(fieldString, "get: function () { return this[SYMBOL_FOR_");
  strcat(fieldString, fieldData->name);
  strcat(fieldString, "] == void(0) ? ");
  strcat(fieldString, fieldData->value ? fieldData->value : "null");
  strcat(fieldString, " : this[SYMBOL_FOR_");
  strcat(fieldString, fieldData->name);
  strcat(fieldString, "]");
  strcat(fieldString, "; },\n");

  // setter
  TS_push_indent_string(fieldString, fieldIndent);

  strcat(fieldString, "set: function (value) { return this[SYMBOL_FOR_");
  strcat(fieldString, fieldData->name);
  strcat(fieldString, "] = value; }\n");

  TS_push_indent_string(fieldString, outputSettings.indent);

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
  TS_push_indent_string(string, outputSettings.indent);

  strcat(string, "(function (C, P) {\n");

  // body
  TS_push_indent_string(string, settings.indent);

  strcat(string, "var proto = {};\n");

  TS_push_indent_string(string, settings.indent);

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

  TS_push_indent_string(string, settings.indent);

  strcat(string, "C.prototype = Object.create(P.prototype, proto);\n");

  // end of function
  TS_push_indent_string(string, outputSettings.indent);

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

  TS_push_indent_string(string, outputSettings.indent);

  strcat(string, "/* class */\n");

  TS_push_indent_string(string, outputSettings.indent);

  strcat(string, "function ");
  strcat(string, data->name);
  strcat(string, "() {\n");

  TS_push_indent_string(string, outputSettings.indent);

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