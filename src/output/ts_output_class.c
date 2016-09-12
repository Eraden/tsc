#include <tsc/output.h>

// PRINT

static void
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_print_for_class_method(
    TSFile *tsFile,
    const TSOutputSettings __attribute__((__weak__)) outputSettings,
    const TSParserToken *child
) {
  const TSFunctionData *methodData = (*child).data;
  const u_long methodIndent = outputSettings.indent + 1;

  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "proto['");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%ls", methodData->name);
  fprintf(outputSettings.stream, "%s", "'] = {\n");
  fflush(outputSettings.stream);

  TS_print_indent(outputSettings.stream, methodIndent);

  fprintf(outputSettings.stream, "%s", "value: function () {\n");
  fflush(outputSettings.stream);

  TSParserToken *body;
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
    TSFile *__attribute__(( __unused__ )) tsFile,
    const TSOutputSettings __attribute__((__weak__)) outputSettings,
    const TSParserToken *child
) {
  const TSLocalVariableData *fieldData = (*child).data;
  const u_long fieldIndent = outputSettings.indent + 1;

  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "var SYMBOL_FOR_");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%ls", fieldData->name);
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", " = Symbol();\n");
  fflush(outputSettings.stream);

  TS_print_indent(outputSettings.stream, outputSettings.indent);

  fprintf(outputSettings.stream, "%s", "proto['");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%ls", fieldData->name);
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", "'] = {\n");
  fflush(outputSettings.stream);

  // getter
  TS_print_indent(outputSettings.stream, fieldIndent);

  fprintf(outputSettings.stream, "%s", "get: function () { return this[SYMBOL_FOR_");
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%ls", fieldData->name);
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%s", "] == void(0) ? ");
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%ls", fieldData->value ? fieldData->value : L"null");
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%s", " : this[SYMBOL_FOR_");
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%ls", fieldData->name);
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%s", "]");
  fflush(outputSettings.stream);

  fprintf(outputSettings.stream, "%s", "; },\n");
  fflush(outputSettings.stream);

  // setter
  TS_print_indent(outputSettings.stream, fieldIndent);

  fprintf(outputSettings.stream, "%s", "set: function (value) { return this[SYMBOL_FOR_");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%ls", fieldData->name);
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
    TSFile *__attribute__(( __unused__ )) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  const TSClassData *data = tsParserToken->data;
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

  for (u_long childIndex = 0; childIndex < tsParserToken->childrenSize; childIndex++) {
    TSParserToken *child = tsParserToken->children[childIndex];
    switch (child->tokenType) {
      case TS_CLASS_FIELD: {
        TS_print_for_class_field(tsFile, settings, child);
        break;
      }
      case TS_CLASS_METHOD: {
        TS_print_for_class_method(tsFile, settings, child);
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
  fprintf(outputSettings.stream, "%ls", (wchar_t *) data->name);
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", ", ");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%ls", (wchar_t *) data->parentClass ? data->parentClass : L"Object");
  fflush(outputSettings.stream);
  fprintf(outputSettings.stream, "%s", "));\n");
  fflush(outputSettings.stream);
}

static void
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_print_for_class_constructor(
    TSFile *__attribute__(( __unused__ )) tsFile,
    const TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  const TSClassData *data = tsParserToken->data;
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
  fprintf(outputSettings.stream, "%ls", (wchar_t *) data->name);
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
    TSFile *__attribute__(( __unused__ )) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  if (tsParserToken->data == NULL) return;
  TS_print_for_class_constructor(tsFile, tsParserToken, outputSettings);
  TS_print_for_class_prototype(tsFile, tsParserToken, outputSettings);
}

// STRING

static const wchar_t *
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
                  wcslen((const wchar_t *) L"proto['") +
                  wcslen(methodData->name) +
                  wcslen((wchar_t *) L"'] = {\n") +
                  (methodIndent * 2) +
                  wcslen((wchar_t *) L"value: function () {\n") +
                  (methodIndent * 2) +
                  wcslen((wchar_t *) L"}\n") +
                  (outputSettings.indent * 2) +
                  wcslen((wchar_t *) L"};\n");

  wchar_t *methodString = (wchar_t *) calloc(sizeof(wchar_t), *methodSize);

  TS_push_indent_string(methodString, outputSettings.indent);

  wcscat(methodString, (wchar_t *) L"proto['");
  wcscat(methodString, methodData->name);
  wcscat(methodString, (wchar_t *) L"'] = {\n");

  TS_push_indent_string(methodString, methodIndent);

  wcscat(methodString, (wchar_t *) L"value: function () {\n");

  TS_push_indent_string(methodString, methodIndent);

  wcscat(methodString, (wchar_t *) L"}\n");

  TS_push_indent_string(methodString, outputSettings.indent);

  wcscat(methodString, (wchar_t *) L"};\n");
  return methodString;
}

static const wchar_t *
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
                 wcslen((wchar_t *) L"var SYMBOL_FOR_") +
                 wcslen(fieldData->name) +
                 wcslen((wchar_t *) L" = Symbol();\n") +
                 (outputSettings.indent * 2) +
                 wcslen((wchar_t *) L"proto['") +
                 wcslen(fieldData->name) +
                 wcslen((wchar_t *) L"'] = {\n") +
                 (fieldIndent * 2) +
                 wcslen((wchar_t *) L"get: function () { return this[SYMBOL_FOR_") +
                 wcslen(fieldData->name) +
                 wcslen((wchar_t *) L"] == void(0) ? ") +
                 wcslen(fieldData->value ? (void *) fieldData->value : "null") +
                 wcslen((wchar_t *) L" : this[SYMBOL_FOR_") +
                 wcslen(fieldData->name) +
                 wcslen((wchar_t *) L"]") +
                 wcslen((wchar_t *) L"; },\n") +
                 (fieldIndent * 2) +
                 wcslen((wchar_t *) L"set: function (value) { return this[SYMBOL_FOR_") +
                 wcslen(fieldData->name) +
                 wcslen((wchar_t *) L"] = value; }\n") +
                 (outputSettings.indent * 2) +
                 wcslen((wchar_t *) L"};\n");

  wchar_t *fieldString = (wchar_t *) calloc(sizeof(wchar_t), *fieldSize);

  TS_push_indent_string(fieldString, outputSettings.indent);

  wcscat(fieldString, (wchar_t *) L"var SYMBOL_FOR_");
  wcscat(fieldString, fieldData->name);
  wcscat(fieldString, (wchar_t *) L" = Symbol();\n");

  TS_push_indent_string(fieldString, outputSettings.indent);

  wcscat(fieldString, (wchar_t *) L"proto['");
  wcscat(fieldString, fieldData->name);
  wcscat(fieldString, (wchar_t *) L"'] = {\n");

  // getter
  for (u_long indentIndex = 0; indentIndex < fieldIndent; indentIndex++)
    wcscat(fieldString, (wchar_t *) L"  ");
  wcscat(fieldString, (wchar_t *) L"get: function () { return this[SYMBOL_FOR_");
  wcscat(fieldString, fieldData->name);
  wcscat(fieldString, (wchar_t *) L"] == void(0) ? ");
  wcscat(fieldString, fieldData->value ? (void *) fieldData->value : "null");
  wcscat(fieldString, (wchar_t *) L" : this[SYMBOL_FOR_");
  wcscat(fieldString, fieldData->name);
  wcscat(fieldString, (wchar_t *) L"]");
  wcscat(fieldString, (wchar_t *) L"; },\n");

  // setter
  TS_push_indent_string(fieldString, fieldIndent);

  wcscat(fieldString, (wchar_t *) L"set: function (value) { return this[SYMBOL_FOR_");
  wcscat(fieldString, fieldData->name);
  wcscat(fieldString, (wchar_t *) L"] = value; }\n");

  TS_push_indent_string(fieldString, outputSettings.indent);

  wcscat(fieldString, (wchar_t *) L"};\n");
  return fieldString;
}

static const wchar_t *
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_string_for_class_prototype(
    TSFile *__attribute__(( __unused__ )) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  const TSClassData *data = tsParserToken->data;
  TSOutputSettings settings = outputSettings;
  settings.indent += 1;
  wchar_t *string = NULL;

  u_long size = TS_STRING_END +
                // begin function
                (outputSettings.indent * 2) +
                wcslen((wchar_t *) L"(function (C, P) {\n") +
                (settings.indent * 2) +
                // body
                wcslen((wchar_t *) L"var proto = {};\n") +
                (settings.indent * 2) +
                wcslen((wchar_t *) L"proto['constructor'] = { value: P };\n") +
                (settings.indent * 2) +
                wcslen((wchar_t *) L"C.prototype = Object.create(P.prototype, proto);\n") +
                // end of function
                (outputSettings.indent * 2) +
                wcslen((wchar_t *) L"}(") +
                wcslen(data->name) +
                wcslen((wchar_t *) L", ") +
                wcslen(data->parentClass ? (void *) data->parentClass : "Object") +
                wcslen((wchar_t *) L"));\n");

  string = (wchar_t *) calloc(sizeof(wchar_t), size);

  // begin function
  TS_push_indent_string(string, outputSettings.indent);

  wcscat(string, (wchar_t *) L"(function (C, P) {\n");

  // body
  TS_push_indent_string(string, settings.indent);

  wcscat(string, (wchar_t *) L"var proto = {};\n");

  TS_push_indent_string(string, settings.indent);

  wcscat(string, (wchar_t *) L"proto['constructor'] = { value: P };\n");

  for (u_long childIndex = 0; childIndex < tsParserToken->childrenSize; childIndex++) {
    TSParserToken *child = tsParserToken->children[childIndex];
    switch (child->tokenType) {
      case TS_CLASS_FIELD: {
        u_long fieldSize;
        const wchar_t *fieldString = TS_string_for_class_field(settings, child, &fieldSize);

        wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size + fieldSize - 1);
        wcscpy(newPointer, string);
        wcscat(newPointer, fieldString);
        free(string);
        free((void *) fieldString);
        string = newPointer;
        size = size + fieldSize - 1;
        break;
      }
      case TS_CLASS_METHOD: {
        u_long methodSize;
        const wchar_t *methodString = TS_string_for_class_method(settings, child, &methodSize);

        wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size + methodSize - 1);
        wcscpy(newPointer, string);
        wcscat(newPointer, methodString);
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

  wcscat(string, (wchar_t *) L"C.prototype = Object.create(P.prototype, proto);\n");

  // end of function
  TS_push_indent_string(string, outputSettings.indent);

  wcscat(string, (wchar_t *) L"}(");
  wcscat(string, data->name);
  wcscat(string, (wchar_t *) L", ");
  wcscat(string, data->parentClass ? (void *) data->parentClass : "Object");
  wcscat(string, (wchar_t *) L"));\n");

  return string;
}

static const wchar_t *
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_string_for_class_constructor(
    TSFile *__attribute__(( __unused__ )) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  const TSClassData *data = tsParserToken->data;
  const u_long __attribute__((__unused__)) indent = outputSettings.indent + 1;
  wchar_t *string = NULL;

  u_long size = 0;
  size = TS_STRING_END +
         (outputSettings.indent * 2 * sizeof(wchar_t)) +
         wcslen((wchar_t *) L"/* class */\n") +
         (outputSettings.indent * 2 * sizeof(wchar_t)) +
         wcslen((wchar_t *) L"function ") +
         wcslen(data->name) +
         wcslen((wchar_t *) L"() {\n") +
         (outputSettings.indent * 2 * sizeof(wchar_t)) +
         wcslen((wchar_t *) L"}\n");

  string = (wchar_t *) calloc(sizeof(wchar_t), size);

  TS_push_indent_string(string, outputSettings.indent);

  wcscat(string, (wchar_t *) L"/* class */\n");

  TS_push_indent_string(string, outputSettings.indent);

  wcscat(string, (wchar_t *) L"function ");
  wcscat(string, data->name);
  wcscat(string, (wchar_t *) L"() {\n");

  TS_push_indent_string(string, outputSettings.indent);

  wcscat(string, (wchar_t *) L"}\n");

  return string;
}

const wchar_t *
__attribute__(( section("output-class")))
TS_string_for_class(
    TSFile *__attribute__(( __unused__ )) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  if (tsParserToken->data == NULL) return NULL;
  wchar_t *string = NULL;

  {
    const wchar_t *constructorString = TS_string_for_class_constructor(tsFile, tsParserToken, outputSettings);
    if (constructorString != NULL) {
      wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), TS_STRING_END + wcslen(constructorString));
      wcscat(newPointer, constructorString);
      free((void *) constructorString);
      string = newPointer;
    }
  }

  {
    const wchar_t *prototypeString = TS_string_for_class_prototype(tsFile, tsParserToken, outputSettings);
    if (prototypeString != NULL) {
      wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t),
                                               TS_STRING_END + wcslen(string) + wcslen(prototypeString));
      wcscpy(newPointer, string);
      wcscat(newPointer, prototypeString);
      free(string);
      free((void *) prototypeString);
      string = newPointer;
    }
  }

  return string;
}