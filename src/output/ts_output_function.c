#include <tsc/output.h>

// PRINT

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_print_for_function_head(
    const TSFile *__attribute__((__unused__)) tsFile,
    const TSFunctionData *data,
    const u_long indent,
    TSOutputSettings outputSettings
) {
  for (u_long i = 0, l = indent; i < l; i++) fprintf(outputSettings.stream, "%s", "  ");
  fprintf(outputSettings.stream, "%s", "function ");
  fprintf(outputSettings.stream, "%ls", (wchar_t *) data->name);
  fprintf(outputSettings.stream, "%s", "(");

  TSLocalVariableData *arg;
  for (u_long argumentIndex = 0, l = data->argumentsSize; argumentIndex < l; argumentIndex++) {
    TSParserToken tsArgumentToken = data->arguments[argumentIndex];
    if (tsArgumentToken.tokenType == TS_VAR && tsArgumentToken.data != NULL) {
      arg = tsArgumentToken.data;
      u_long argSize = sizeof(arg->name) + 1;
      u_char isSpread = (u_char) (argSize > 3 && arg->name[0] == '.' && arg->name[1] == '.' && arg->name[2] == '.');
      if (isSpread) {
      } else if (argumentIndex > 0) {
        fprintf(outputSettings.stream, "%s", ", ");
      }
      if (isSpread) {
        fprintf(outputSettings.stream, "%s", "  /* spread */");
      } else {
        fprintf(outputSettings.stream, "%ls", (wchar_t *) arg->name);
      }
    }
  }
  fprintf(outputSettings.stream, "%s", ") {\n");
}

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_print_for_function_spread_arg(
    const u_long indent,
    const TSFunctionData *data,
    const TSLocalVariableData *arg,
    TSOutputSettings outputSettings
) {
  const wchar_t *name;
  u_long argSize = sizeof(arg->name) + TS_STRING_END - TS_SPREAD_PREFIX_SIZE;
  wchar_t *tmp = (wchar_t *) calloc(sizeof(wchar_t), argSize + 1);
  for (u_long cIndex = 0; cIndex < argSize; cIndex++) {
    tmp[cIndex] = arg->name[cIndex + 3];
  }
  name = tmp;
  const int n = snprintf(NULL, 0, "%lu", data->argumentsSize - 1);
  char *numberOfArguments = (char *) calloc(sizeof(char), (unsigned) n + 1);
  snprintf(numberOfArguments, (unsigned) n + 1, "%lu", data->argumentsSize - 1);
  for (u_long indentIndex = 0; indentIndex < indent + 1; indentIndex++)
    fprintf(outputSettings.stream, "%s", "  ");
  fprintf(outputSettings.stream, "%s", "if (");
  fprintf(outputSettings.stream, "%ls", (wchar_t *) name);
  fprintf(outputSettings.stream, "%s", " == void(0)) ");
  fprintf(outputSettings.stream, "%ls", (wchar_t *) name);
  fprintf(outputSettings.stream, "%s", " = [].slice(arguments, ");
  fprintf(outputSettings.stream, "%s", numberOfArguments);
  fprintf(outputSettings.stream, "%s", ");\n");
  free((void *) name);
  free((void *) numberOfArguments);
}

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_print_for_function_arg_default(
    const u_long indent,
    const TSFunctionData *__attribute__((__unused__)) data,
    const TSLocalVariableData *arg,
    TSOutputSettings outputSettings
) {
  if (arg->value == NULL) return;
  for (u_long indentIndex = 0; indentIndex < indent + 1; indentIndex++) {
    fprintf(outputSettings.stream, "%s", "  ");
  }
  fprintf(outputSettings.stream, "%s", "if (");
  fprintf(outputSettings.stream, "%ls", (wchar_t *) arg->name);
  fprintf(outputSettings.stream, "%s", " == void(0)) ");
  fprintf(outputSettings.stream, "%ls", (wchar_t *) arg->name);
  fprintf(outputSettings.stream, "%s", " = ");
  fprintf(outputSettings.stream, "%ls", (wchar_t *) arg->value);
  fprintf(outputSettings.stream, "%s", ";\n");
}

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_print_for_function_body(
    const TSFile *tsFile,
    const TSFunctionData *data,
    const u_long indent,
    const TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  TSLocalVariableData *arg;
  volatile u_char hadSpread = 0;
  for (u_long argumentIndex = 0; argumentIndex < data->argumentsSize; argumentIndex++) {
    TSParserToken tsArgumentToken = data->arguments[argumentIndex];
    if (tsArgumentToken.tokenType == TS_VAR && tsArgumentToken.data != NULL) {
      arg = tsArgumentToken.data;
      u_long argSize = sizeof(arg->name);
      u_char isSpread = (u_char) (argSize > 3 && arg->name[0] == '.' && arg->name[1] == '.' && arg->name[2] == '.');
      if (isSpread && hadSpread) {
        ts_syntax_error((wchar_t *) L"Spread argument declared twice!", tsFile->file, tsParserToken->character, tsParserToken->line);
      }
      if (!hadSpread) hadSpread = isSpread;
      if (isSpread) {
        TS_print_for_function_spread_arg(indent, data, arg, outputSettings);
      } else {
        TS_print_for_function_arg_default(indent, data, arg, outputSettings);
      }
    }
  }

  for (u_long childIndex = 0; childIndex < tsParserToken->childrenSize; childIndex++) {
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    TS_print_for_token(tsFile, tsParserToken->children[childIndex], settings);
  }
}

void
__attribute__(( section("output-function")))
TS_print_from_function(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  const u_long indent = outputSettings.indent;
  const TSFunctionData *data = tsParserToken.data;
  if (data == NULL) return;
  TS_print_for_function_head(tsFile, data, indent, outputSettings);
  TS_print_for_function_body(tsFile, data, indent, &tsParserToken, outputSettings);

  for (u_long index = 0; index < indent; index++)
    fprintf(outputSettings.stream, "%s", "  ");
  fprintf(outputSettings.stream, "%s", "}\n");
}

// STRING

static u_long
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_output_measure_function_head(
    const TSFile *__attribute__((__unused__)) tsFile,
    const u_long indent,
    const TSFunctionData *data
) {
  u_long size = sizeof(wchar_t) * (indent * 2);
  size += sizeof("function ");
  size += sizeof(data->name);
  size += sizeof("(");
  TSLocalVariableData *arg;
  for (u_long i = 0, l = data->argumentsSize; i < l; i++) {
    TSParserToken tsArgumentToken = data->arguments[i];
    if (tsArgumentToken.tokenType == TS_VAR && tsArgumentToken.data != NULL) {
      arg = tsArgumentToken.data;
      u_long argSize = sizeof(arg->name);
      u_char isSpread = (u_char) (argSize > 3 && arg->name[0] == '.' && arg->name[1] == '.' && arg->name[2] == '.');
      if (isSpread) {
        argSize = wcslen((wchar_t *) L"  /* spread */");
      }
      if (i > 0 && isSpread == 0) size += wcslen((wchar_t *) L", ");
      size += argSize;
    }
  }
  size += wcslen((wchar_t *) L") {\n");
  return size;
}

static wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_string_for_function_head(
    const TSFile *__attribute__((__unused__)) tsFile,
    const TSFunctionData *data,
    const u_long indent,
    const u_long size
) {
  wchar_t *string = (wchar_t *) calloc(sizeof(wchar_t), size + 1);
  for (u_long i = 0, l = indent; i < l; i++) wcscat(string, (wchar_t *) L"  ");
  wcscat(string, (wchar_t *) L"function ");
  wcscat(string, data->name);
  wcscat(string, (wchar_t *) L"(");

  TSLocalVariableData *arg;
  for (u_long argumentIndex = 0, l = data->argumentsSize; argumentIndex < l; argumentIndex++) {
    TSParserToken tsArgumentToken = data->arguments[argumentIndex];
    if (tsArgumentToken.tokenType == TS_VAR && tsArgumentToken.data != NULL) {
      arg = tsArgumentToken.data;
      u_long argSize = sizeof(arg->name);
      u_char isSpread = (u_char) (argSize > 3 && arg->name[0] == '.' && arg->name[1] == '.' && arg->name[2] == '.');
      if (isSpread) {
        argSize = wcslen((wchar_t *) L"  /* spread */") + 1;
      } else if (argumentIndex > 0) {
        wcscat(string, (wchar_t *) L", ");
      }
      wchar_t *name = (wchar_t *) calloc(sizeof(wchar_t), argSize);
      if (isSpread) {
        wcscpy(name, (const wchar_t *) L"  /* spread */");
      } else {
        wcscpy(name, arg->name);
      }
      name[argSize - 1] = 0;
      wcscat(string, name);
      free(name);
    }
  }
  wcscat(string, (wchar_t *) L") {\n");
  return string;
}

static wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_string_for_function_spread_arg(
    const u_long indent,
    const TSFunctionData *data,
    const TSLocalVariableData *arg
) {
  const wchar_t *name;
  u_long variableSize;
  u_long argSize = sizeof(arg->name) + TS_STRING_END - TS_SPREAD_PREFIX_SIZE;
  wchar_t *tmp = (wchar_t *) calloc(sizeof(wchar_t), argSize + 1);
  for (u_long cIndex = 0; cIndex < argSize; cIndex++) {
    tmp[cIndex] = arg->name[cIndex + 3];
  }
  name = tmp;
  const int n = snprintf(NULL, 0, "%lu", data->argumentsSize - 1);
  wchar_t *numberOfArguments = (wchar_t *) calloc(sizeof(wchar_t), (unsigned) n + 1);
  swprintf(numberOfArguments, (unsigned) n + 1, (const wchar_t *) L"%lu", data->argumentsSize - 1);
  variableSize = ((indent + 1) * 2) +
                 wcslen((wchar_t *) L"if (") +
                 wcslen(name) +
                 wcslen((wchar_t *) L" == void(0)) ") +
                 wcslen(name) +
                 wcslen((wchar_t *) L" = [].slice(arguments, ") +
                 wcslen(numberOfArguments) +
                 wcslen((wchar_t *) L");\n");
  wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), variableSize + 1);
  for (u_long indentIndex = 0; indentIndex < indent + 1; indentIndex++) wcscat(newPointer, (wchar_t *) L"  ");
  wcscat(newPointer, (wchar_t *) L"if (");
  wcscat(newPointer, name);
  wcscat(newPointer, (wchar_t *) L" == void(0)) ");
  wcscat(newPointer, name);
  wcscat(newPointer, (wchar_t *) L" = [].slice(arguments, ");
  wcscat(newPointer, numberOfArguments);
  wcscat(newPointer, (wchar_t *) L");\n");
  free((void *) name);
  free((void *) numberOfArguments);
  return newPointer;
}

static wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_string_for_function_arg_default(
    const u_long indent,
    const TSFunctionData *__attribute__((__unused__)) data,
    const TSLocalVariableData *arg
) {
  if (arg->value == NULL) return NULL;
  const wchar_t *name = TS_clone_string(arg->name);
  const u_long nameSize = wcslen(name);
  u_long size = TS_STRING_END +
                ((indent + 1) * 2) +
                wcslen((wchar_t *) L"if (") +
                nameSize +
                wcslen((wchar_t *) L" == void(0)) ") +
                nameSize +
                wcslen((wchar_t *) L" = ") +
                wcslen(arg->value) +
                wcslen((wchar_t *) L"}\n");
  wchar_t *defaultValue = (wchar_t *) calloc(sizeof(wchar_t), size);
  for (u_long indentIndex = 0; indentIndex < indent + 1; indentIndex++) {
    wcscat(defaultValue, (wchar_t *) L"  ");
  }
  wcscat(defaultValue, (wchar_t *) L"if (");
  wcscat(defaultValue, name);
  wcscat(defaultValue, (wchar_t *) L" == void(0)) ");
  wcscat(defaultValue, name);
  wcscat(defaultValue, (wchar_t *) L" = ");
  wcscat(defaultValue, arg->value);
  wcscat(defaultValue, (wchar_t *) L";\n");
  free((wchar_t *) name);
  return defaultValue;
}

static wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_string_for_function_body(
    const TSFile *tsFile,
    const TSFunctionData *data,
    const u_long indent,
    wchar_t *string,
    const TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  TSLocalVariableData *arg;
  volatile u_char hadSpread = 0;
  for (u_long argumentIndex = 0; argumentIndex < data->argumentsSize; argumentIndex++) {
    TSParserToken tsArgumentToken = data->arguments[argumentIndex];
    if (tsArgumentToken.tokenType == TS_VAR && tsArgumentToken.data != NULL) {
      arg = tsArgumentToken.data;
      u_long argSize = sizeof(arg->name);
      u_char isSpread = (u_char) (argSize > 3 && arg->name[0] == '.' && arg->name[1] == '.' && arg->name[2] == '.');
      if (isSpread && hadSpread) {
        ts_syntax_error((wchar_t *) L"Spread argument declared twice!", tsFile->file, tsParserToken->character, tsParserToken->line);
      }
      if (!hadSpread) hadSpread = isSpread;
      if (isSpread) {
        wchar_t *spreadArg = TS_string_for_function_spread_arg(indent, data, arg);
        wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), wcslen(string) + wcslen(spreadArg) + 1);
        wcscpy(newPointer, string);
        wcscat(newPointer, spreadArg);
        free(string);
        string = newPointer;
      } else {
        wchar_t *defaultValue = TS_string_for_function_arg_default(indent, data, arg);
        if (defaultValue != NULL) {
          const u_long defaultValueSize = wcslen(defaultValue);
          const u_long stringSize = wcslen(string);
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), stringSize + defaultValueSize + 1);
          wcscpy(newPointer, string);
          wcscat(newPointer, defaultValue);
          free(string);
          string = newPointer;
        }
      }
    }
  }

  for (u_long childIndex = 0; childIndex < tsParserToken->childrenSize; childIndex++) {
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    const wchar_t *childString = TS_string_for_token(tsFile, tsParserToken->children[childIndex], settings);
    if (childString != NULL) {
      wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), wcslen(string) + wcslen(childString) + 1);
      wcscpy(newPointer, string);
      wcscat(newPointer, childString);
      free(string);
      string = newPointer;
    }
  }
  return string;
}

const wchar_t *
__attribute__(( section("output-function")))
TS_string_from_function(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  const u_long indent = outputSettings.indent;
  const TSFunctionData *data = tsParserToken.data;
  if (data == NULL) return NULL;
  wchar_t *string = NULL;
  u_long size = TS_output_measure_function_head(tsFile, indent, data);
  string = TS_string_for_function_head(tsFile, data, indent, size + 1);
  string = TS_string_for_function_body(tsFile, data, indent, string, &tsParserToken, outputSettings);

  wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), wcslen(string) + (indent * 2) + wcslen((wchar_t *) L"}\n") + TS_STRING_END);
  wcscpy(newPointer, string);
  free(string);
  string = newPointer;

  for (u_long index = 0; index < indent; index++) wcscat(string, (wchar_t *) L"  ");
  wcscat(string, (wchar_t *) L"}\n");
  return string;
}

