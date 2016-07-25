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
  fprintf(outputSettings.stream, "%s", data->name);
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
        fprintf(outputSettings.stream, "%s", arg->name);
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
  const char *name;
  u_long argSize = sizeof(arg->name) + TS_STRING_END - TS_SPREAD_PREFIX_SIZE;
  char *tmp = (char *) calloc(sizeof(char), argSize + 1);
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
  fprintf(outputSettings.stream, "%s", name);
  fprintf(outputSettings.stream, "%s", " == void(0)) ");
  fprintf(outputSettings.stream, "%s", name);
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
  fprintf(outputSettings.stream, "%s", arg->name);
  fprintf(outputSettings.stream, "%s", " == void(0)) ");
  fprintf(outputSettings.stream, "%s", arg->name);
  fprintf(outputSettings.stream, "%s", " = ");
  fprintf(outputSettings.stream, "%s", arg->value);
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
        ts_syntax_error("Spread argument declared twice!", tsFile->file, tsParserToken->character, tsParserToken->line);
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
  fprintf(outputSettings.stream, "%s", "}");
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
  u_long size = sizeof(char) * (indent * 2);
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
        argSize = strlen("  /* spread */");
      }
      if (i > 0 && isSpread == 0) size += sizeof(", ");
      size += argSize;
    }
  }
  size += sizeof(") {\n");
  return size;
}

static char *
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_string_for_function_head(
    const TSFile *__attribute__((__unused__)) tsFile,
    const TSFunctionData *data,
    const u_long indent,
    const u_long size
) {
  char *string = (char *) calloc(sizeof(char), size + 1);
  for (u_long i = 0, l = indent; i < l; i++) strcat(string, "  ");
  strcat(string, "function ");
  strcat(string, data->name);
  strcat(string, "(");

  TSLocalVariableData *arg;
  for (u_long argumentIndex = 0, l = data->argumentsSize; argumentIndex < l; argumentIndex++) {
    TSParserToken tsArgumentToken = data->arguments[argumentIndex];
    if (tsArgumentToken.tokenType == TS_VAR && tsArgumentToken.data != NULL) {
      arg = tsArgumentToken.data;
      u_long argSize = sizeof(arg->name);
      u_char isSpread = (u_char) (argSize > 3 && arg->name[0] == '.' && arg->name[1] == '.' && arg->name[2] == '.');
      if (isSpread) {
        argSize = strlen("  /* spread */") + 1;
      } else if (argumentIndex > 0) {
        strcat(string, ", ");
      }
      char *name = (char *) calloc(sizeof(char), argSize);
      if (isSpread) {
        strcpy(name, "  /* spread */");
      } else {
        strcpy(name, arg->name);
      }
      name[argSize - 1] = 0;
      strcat(string, name);
      free(name);
    }
  }
  strcat(string, ") {\n");
  return string;
}

static char *
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_string_for_function_spread_arg(
    const u_long indent,
    const TSFunctionData *data,
    const TSLocalVariableData *arg
) {
  const char *name;
  u_long variableSize;
  u_long argSize = sizeof(arg->name) + TS_STRING_END - TS_SPREAD_PREFIX_SIZE;
  char *tmp = (char *) calloc(sizeof(char), argSize + 1);
  for (u_long cIndex = 0; cIndex < argSize; cIndex++) {
    tmp[cIndex] = arg->name[cIndex + 3];
  }
  name = tmp;
  const int n = snprintf(NULL, 0, "%lu", data->argumentsSize - 1);
  char *numberOfArguments = (char *) calloc(sizeof(char), (unsigned) n + 1);
  snprintf(numberOfArguments, (unsigned) n + 1, "%lu", data->argumentsSize - 1);
  variableSize = ((indent + 1) * 2) +
                 strlen("if (") +
                 strlen(name) +
                 strlen(" == void(0)) ") +
                 strlen(name) +
                 strlen(" = [].slice(arguments, ") +
                 strlen(numberOfArguments) +
                 strlen(");\n");
  char *newPointer = (char *) calloc(sizeof(char), variableSize + 1);
  for (u_long indentIndex = 0; indentIndex < indent + 1; indentIndex++) strcat(newPointer, "  ");
  strcat(newPointer, "if (");
  strcat(newPointer, name);
  strcat(newPointer, " == void(0)) ");
  strcat(newPointer, name);
  strcat(newPointer, " = [].slice(arguments, ");
  strcat(newPointer, numberOfArguments);
  strcat(newPointer, ");\n");
  free((void *) name);
  free((void *) numberOfArguments);
  return newPointer;
}

static char *
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_string_for_function_arg_default(
    const u_long indent,
    const TSFunctionData *__attribute__((__unused__)) data,
    const TSLocalVariableData *arg
) {
  if (arg->value == NULL) return NULL;
  const char *name = TS_clone_string(arg->name);
  const u_long nameSize = strlen(name);
  u_long size = TS_STRING_END +
                ((indent + 1) * 2) +
                strlen("if (") +
                nameSize +
                strlen(" == void(0)) ") +
                nameSize +
                strlen(" = ") +
                strlen(arg->value) +
                strlen("}\n");
  char *defaultValue = (char *) calloc(sizeof(char), size);
  for (u_long indentIndex = 0; indentIndex < indent + 1; indentIndex++) {
    strcat(defaultValue, "  ");
  }
  strcat(defaultValue, "if (");
  strcat(defaultValue, name);
  strcat(defaultValue, " == void(0)) ");
  strcat(defaultValue, name);
  strcat(defaultValue, " = ");
  strcat(defaultValue, arg->value);
  strcat(defaultValue, ";\n");
  free((char *) name);
  return defaultValue;
}

static char *
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_string_for_function_body(
    const TSFile *tsFile,
    const TSFunctionData *data,
    const u_long indent,
    char *string,
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
        ts_syntax_error("Spread argument declared twice!", tsFile->file, tsParserToken->character, tsParserToken->line);
      }
      if (!hadSpread) hadSpread = isSpread;
      if (isSpread) {
        char *spreadArg = TS_string_for_function_spread_arg(indent, data, arg);
        char *newPointer = (char *) calloc(sizeof(char), strlen(string) + strlen(spreadArg) + 1);
        strcpy(newPointer, string);
        strcat(newPointer, spreadArg);
        free(string);
        string = newPointer;
      } else {
        char *defaultValue = TS_string_for_function_arg_default(indent, data, arg);
        if (defaultValue != NULL) {
          const u_long defaultValueSize = strlen(defaultValue);
          const u_long stringSize = strlen(string);
          char *newPointer = (char *) calloc(sizeof(char), stringSize + defaultValueSize + 1);
          strcpy(newPointer, string);
          strcat(newPointer, defaultValue);
          free(string);
          string = newPointer;
        }
      }
    }
  }

  for (u_long childIndex = 0; childIndex < tsParserToken->childrenSize; childIndex++) {
    TSOutputSettings settings = outputSettings;
    settings.indent += 1;
    const char *childString = TS_string_for_token(tsFile, tsParserToken->children[childIndex], settings);
    if (childString != NULL) {
      char *newPointer = (char *) calloc(sizeof(char), strlen(string) + strlen(childString) + 1);
      strcpy(newPointer, string);
      strcat(newPointer, childString);
      free(string);
      string = newPointer;
    }
  }
  return string;
}

const char *
__attribute__(( section("output-function")))
TS_string_from_function(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  const u_long indent = outputSettings.indent;
  const TSFunctionData *data = tsParserToken.data;
  if (data == NULL) return NULL;
  char *string = NULL;
  u_long size = TS_output_measure_function_head(tsFile, indent, data);
  string = TS_string_for_function_head(tsFile, data, indent, size + 1);
  string = TS_string_for_function_body(tsFile, data, indent, string, &tsParserToken, outputSettings);

  char *newPointer = (char *) calloc(sizeof(char), strlen(string) + (indent * 2) + strlen("}") + 1);
  strcpy(newPointer, string);
  free(string);
  string = newPointer;

  for (u_long index = 0; index < indent; index++) strcat(string, "  ");
  strcat(string, "}");
  return string;
}

