#include <tsc/parser.h>

static void
__attribute__(( visibility("hidden")))
TS_append_argument(
    TSFunctionData *data,
    TSParserToken argument
) {
  if (data == NULL) return;
  TSParserToken *newPointer = calloc(sizeof(TSParserToken), data->argumentsSize + 1);
  if (data->arguments != NULL) memcpy(newPointer, data->arguments, sizeof(TSParserToken) * data->argumentsSize);
  if (data->arguments != NULL) free(data->arguments);
  data->arguments = newPointer;
  data->arguments[data->argumentsSize] = argument;
  data->argumentsSize += 1;
}

static const TSParserToken
__attribute__((visibility("hidden")))
TS_parse_function_argument(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  const char *tok;
  TSParserToken argument;
  u_long movedBy = 0;

  TSLocalVariableData *argumentData = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);
  argumentData->name = NULL;
  argumentData->value = NULL;
  argumentData->type = NULL;

  argument.character = tsParseData->character;
  argument.position = tsParseData->position;
  argument.line = tsParseData->line;
  argument.tokenType = TS_VAR;
  argument.data = argumentData;
  argument.visibility = TS_VISIBILITY_SCOPE;
  argument.children = NULL;
  argument.childrenSize = 0;

  volatile unsigned char proceed = 1;
  TSFunctionParseFlag parseFlag = TS_PARSE_FN_ARG_NAME;

  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);

    log_to_file("current token for function argument: '%s'\n", tok);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of function argument", tsFile, &argument);
    }

    switch (tok[0]) {
      case ' ': {
        movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '\n': {
        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;
        free((void *) tok);
        break;
      }
      case ')': {
        if (parseFlag == TS_PARSE_FN_ARG_VALUE && argumentData->value == NULL)
          ts_token_syntax_error("Value for function argument is missing", tsFile, &argument);
        if (parseFlag == TS_PARSE_FN_ARG_TYPE && argumentData->type == NULL)
          ts_token_syntax_error("Value for function argument is missing", tsFile, &argument);
        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      case '=': {
        movedBy += strlen(tok);
        free((void *) tok);

        if (argumentData->name == NULL) {
          ts_token_syntax_error("Assigning to argument without name", tsFile, &argument);
        }
        parseFlag = TS_PARSE_FN_ARG_VALUE;
        break;
      }
      case ',': {
        movedBy += strlen(tok);
        free((void *) tok);

        if (parseFlag == TS_PARSE_FN_ARG_VALUE && argumentData->value == NULL)
          ts_token_syntax_error("Value for function argument is missing", tsFile, &argument);
        if (parseFlag == TS_PARSE_FN_ARG_TYPE && argumentData->type == NULL)
          ts_token_syntax_error("Value for function argument is missing", tsFile, &argument);
        if (argumentData->name == NULL)
          ts_token_syntax_error("Declared argument as next but previous has no name", tsFile, &argument);

        proceed = 0;

        break;
      }
      case ':': {
        movedBy += strlen(tok);
        free((void *) tok);

        if (argumentData->name == NULL)
          ts_token_syntax_error("Declared argument type but argument has no name", tsFile, &argument);
        if (argumentData->type != NULL)
          ts_token_syntax_error("Missing argument type after typing symbol", tsFile, &argument);
        parseFlag = TS_PARSE_FN_ARG_TYPE;

        break;
      }
      default: {
        if (strcmp(tok, "private") == 0) {
          argument.visibility = TS_VISIBILITY_PRIVATE;
        } else if (strcmp(tok, "protected") == 0) {
          argument.visibility = TS_VISIBILITY_PROTECTED;
        } else if (strcmp(tok, "public") == 0) {
          argument.visibility = TS_VISIBILITY_PUBLIC;
        } else if (parseFlag == TS_PARSE_FN_ARG_NAME) {
          u_long size = strlen(tok) + 1;
          if (argumentData->name != NULL) size += strlen(argumentData->name);
          char *newPointer = (char *) calloc(sizeof(char), size);
          if (argumentData->name != NULL) strcpy(newPointer, argumentData->name);
          if (argumentData->name != NULL) free((void *) argumentData->name);
          strcat(newPointer, tok);
          argumentData->name = newPointer;
        } else if (parseFlag == TS_PARSE_FN_ARG_VALUE) {
          u_long size = strlen(tok) + TS_STRING_END;
          if (argumentData->value != NULL) size = size + strlen(argumentData->value) + strlen(" ");
          char *newPointer = (char *) calloc(sizeof(char), size);
          if (argumentData->value != NULL) strcpy(newPointer, argumentData->value);
          if (argumentData->value != NULL) strcat(newPointer, " ");
          if (argumentData->value) free((void *) argumentData->value);
          strcat(newPointer, tok);
          argumentData->value = newPointer;
        } else /*if (parseFlag == TS_PARSE_FN_ARG_TYPE)*/ {
          argumentData->type = TS_clone_string(tok);
        }

        movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  return argument;
}

static void
__attribute__(( visibility("hidden")))
TS_parse_function_arguments(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token
) {
  const char *tok;
  u_long movedBy = 0;
  u_short hadStartBracket = 0;
  volatile unsigned char proceed = 1;

  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of function argument", tsFile, token);
    }

    if (!hadStartBracket) {
      if (tok[0] == '(') {
        hadStartBracket = 1;
        movedBy += strlen(tok);
        free((void *) tok);
        continue;

      } else {
        free((void *) tok);
        ts_token_syntax_error("Function arguments starts before bracket", tsFile, token);
      }
    }

    switch (tok[0]) {
      case ' ': {
        movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '\n': {
        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;
        free((void *) tok);
        break;
      }
      case ')': {
        movedBy += strlen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }

      default: {
        TS_put_back(tsParseData->stream, tok);
        tsParseData->token = tok;
        tsParseData->character += movedBy;
        tsParseData->position += movedBy;
        movedBy = 0;

        TSParserToken arg = TS_parse_function_argument(tsFile, tsParseData);
        TS_append_argument(token->data, arg);

        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
}

static void
__attribute((visibility("hidden")))
TS_parse_function_lookup_return_type(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token
) {
  TSFunctionData *data = token->data;
  const char *tok;
  u_long movedBy = 0;
  volatile unsigned char proceed = 1;

  proceed = 1;
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of stream while looking for function return type", tsFile, token);
    }

    switch (tok[0]) {
      case ' ': {
        movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '\n': {
        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;
        free((void *) tok);
        break;
      }
      case ':': {
        movedBy += strlen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      case '{': {
        movedBy += strlen(tok);
        tsParseData->position += movedBy;
        tsParseData->character += movedBy;
        free((void *) tok);
        return;
      }
      default: {
        ts_token_syntax_error("Unexpected token while looking for function type", tsFile, token);
      }
    }
  }

  proceed = 1;
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of stream while looking for function return type", tsFile, token);
    }

    switch (tok[0]) {
      case ' ': {
        movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '\n': {
        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;
        free((void *) tok);
        break;
      }
      case '{': {
        proceed = 0;
        movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      default: {
        if (data->returnType != NULL)
          ts_token_syntax_error("Unexpected token while parsing function return type. Return type was already defined!",
                                tsFile, token);
        if (!TS_name_is_valid(tok))
          ts_token_syntax_error("Invalid type name for function return type!", tsFile, token);

        data->returnType = TS_clone_string(tok);
        movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
    }
  }
  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
}

static void
__attribute__((visibility("hidden")))
TS_parse_function_body(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token,
    u_long *movedBy
) {
  const char *tok = NULL;
  // move to bracket '{'
  volatile unsigned char proceed;

  proceed = 1;

  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_syntax_error("Unexpected end of function body", tsFile->file, tsParseData->character, tsParseData->line);
    }
    switch (tok[0]) {
      case '\n': {
        *movedBy += strlen(tok);
        free((void *) tok);

        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += *movedBy;
        *movedBy = 0;
        break;
      }
      case ' ': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '}': {
        *movedBy += strlen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        tsParseData->token = tok;

        TSParserToken tsParserToken = TS_parse_ts_token(tsFile, tsParseData);

        *movedBy += strlen(tok);
        free((void *) tok);
        free((void *) tsParseData->token);

        if (tsParserToken.tokenType != TS_UNKNOWN) {
          u_long size = token->childrenSize + 1;
          TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), size);
          if (token->children) memcpy(newPointer, token->children, sizeof(TSParserToken) * token->childrenSize);
          if (token->children) free(token->children);
          token->children = newPointer;
          token->children[token->childrenSize] = tsParserToken;
          token->childrenSize += 1;
        } else {
          free(tsParserToken.data);
        }
        break;
      }
    }
  }
}

const TSParserToken TS_parse_function(TSFile *tsFile, TSParseData *tsParseData) {
  TS_TOKEN_BEGIN("function");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_FUNCTION;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  TSFunctionData *functionData = calloc(sizeof(TSFunctionData), 1);
  functionData->arguments = NULL;
  functionData->argumentsSize = 0;
  functionData->name = NULL;
  functionData->returnType = NULL;
  token.data = functionData;

  const char *tok;
  volatile unsigned char proceed = 1;

  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of stream while parsing function", tsFile, &token);
    }

    switch (tok[0]) {
      case '\n': {
        movedBy += strlen(tok);
        free((void *) tok);

        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;
        break;
      }
      case ' ': {
        movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '(': {
        proceed = 0;
        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);
        break;
      }
      default: {
        if (TS_name_is_valid(tok)) {
          functionData->name = TS_clone_string(tok);
          free((void *) tok);
        } else {
          ts_token_syntax_error("Invalid function name", tsFile, &token);
        }
        proceed = 0;
        break;
      }
    }
  }

  TS_parse_function_arguments(tsFile, tsParseData, &token);
  TS_parse_function_lookup_return_type(tsFile, tsParseData, &token);
  TS_parse_function_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("function");
  return token;
}

void TS_free_function(const TSParserToken token) {
  TS_free_children(token);

  TSFunctionData *data = token.data;
  if (data == NULL) return;
  if (data->name != NULL) free((void *) data->name);
  if (data->returnType != NULL) free((void *) data->returnType);
  if (data->argumentsSize > 0) {
    for (u_long argIndex = 0; argIndex < data->argumentsSize; argIndex++) {
      TS_free_var(data->arguments[argIndex]);
    }
  }
  if (data->arguments != NULL) free(data->arguments);
  free(data);
}
