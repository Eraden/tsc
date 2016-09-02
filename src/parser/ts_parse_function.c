#include <tsc/parser.h>

static void
__attribute__(( visibility("hidden")))
TS_append_argument(
    TSFunctionData *data,
    TSParserToken *argument
) {
  if (data == NULL) return;
  TSParserToken **newPointer = calloc(sizeof(TSParserToken *), data->argumentsSize + 1);
  if (data->arguments != NULL) memcpy(newPointer, data->arguments, sizeof(TSParserToken *) * data->argumentsSize);
  if (data->arguments != NULL) free(data->arguments);
  data->arguments = newPointer;
  data->arguments[data->argumentsSize] = argument;
  data->argumentsSize += 1;
}

static TSParserToken *
__attribute__((visibility("hidden")))
TS_parse_function_argument(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  const wchar_t *tok;
  u_long movedBy = 0;

  TSLocalVariableData *argumentData = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);
  argumentData->name = NULL;
  argumentData->value = NULL;
  argumentData->type = NULL;

  TSParserToken *argument = TS_build_parser_token(TS_ARGUMENT, tsParseData);
  argument->data = argumentData;

  volatile unsigned char proceed = 1;
  TSFunctionParseFlag parseFlag = TS_PARSE_FN_ARG_NAME;

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    log_to_file(
        (wchar_t *) L"current token for function argument: '%ls'\n",
        tok
    );

    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of function argument", tsFile, argument);
    }

    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;
        free((void *) tok);
        break;
      }
      case L')': {
        if (parseFlag == TS_PARSE_FN_ARG_VALUE && argumentData->value == NULL)
          ts_token_syntax_error((wchar_t *) L"Value for function argument is missing", tsFile, argument);

        if (parseFlag == TS_PARSE_FN_ARG_TYPE && argumentData->type == NULL)
          ts_token_syntax_error((wchar_t *) L"Value for function argument is missing", tsFile, argument);

        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      case L'=': {
        movedBy += wcslen(tok);
        free((void *) tok);

        if (argumentData->name == NULL) {
          ts_token_syntax_error((wchar_t *) L"Assigning to argument without name", tsFile, argument);
        }
        parseFlag = TS_PARSE_FN_ARG_VALUE;
        break;
      }
      case L',': {
        movedBy += wcslen(tok);
        free((void *) tok);

        if (parseFlag == TS_PARSE_FN_ARG_VALUE && argumentData->value == NULL)
          ts_token_syntax_error((wchar_t *) L"Value for function argument is missing", tsFile, argument);

        if (parseFlag == TS_PARSE_FN_ARG_TYPE && argumentData->type == NULL)
          ts_token_syntax_error((wchar_t *) L"Value for function argument is missing", tsFile, argument);

        if (argumentData->name == NULL)
          ts_token_syntax_error((wchar_t *) L"Declared argument as next but previous has no name", tsFile, argument);

        proceed = 0;

        break;
      }
      case L':': {
        movedBy += wcslen(tok);
        free((void *) tok);

        if (argumentData->name == NULL)
          ts_token_syntax_error((wchar_t *) L"Found colon but function argument has no name", tsFile, argument);

        if (argumentData->type != NULL)
          ts_token_syntax_error((wchar_t *) L"Missing argument type after typing symbol", tsFile, argument);
        parseFlag = TS_PARSE_FN_ARG_TYPE;

        break;
      }
      default: {
        if (wcscmp(tok, (wchar_t *) L"private") == 0) {
          argument->visibility = TS_VISIBILITY_PRIVATE;

        } else if (wcscmp(tok, (wchar_t *) L"protected") == 0) {
          argument->visibility = TS_VISIBILITY_PROTECTED;

        } else if (wcscmp(tok, (wchar_t *) L"public") == 0) {
          argument->visibility = TS_VISIBILITY_PUBLIC;

        } else if (parseFlag == TS_PARSE_FN_ARG_NAME) {
          u_long size = wcslen(tok) + 1;
          if (argumentData->name != NULL) size += wcslen(argumentData->name);
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
          if (argumentData->name != NULL) wcscpy(newPointer, argumentData->name);
          if (argumentData->name != NULL) free((void *) argumentData->name);
          wcscat(newPointer, tok);
          argumentData->name = newPointer;

        } else if (parseFlag == TS_PARSE_FN_ARG_VALUE) {
          u_long size = wcslen(tok) + TS_STRING_END;
          if (argumentData->value != NULL)
            size = size + wcslen(argumentData->value) + wcslen((const wchar_t *) L" ");
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
          if (argumentData->value != NULL) wcscpy(newPointer, argumentData->value);
          if (argumentData->value != NULL) wcscat(newPointer, (const wchar_t *) L" ");
          if (argumentData->value) free((void *) argumentData->value);
          wcscat(newPointer, tok);
          argumentData->value = newPointer;

        } else /*if (parseFlag == TS_PARSE_FN_ARG_TYPE)*/ {
          argumentData->type = TS_clone_string(tok);
        }

        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  tsParseData->parentTSToken = argument->parent;

  return argument;
}

static void
__attribute__(( visibility("hidden")))
TS_parse_function_arguments(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token
) {
  const wchar_t *tok;
  u_long movedBy = 0;
  u_short hadStartBracket = 0;
  volatile unsigned char proceed = 1;

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of function argument", tsFile, token);
    }

    if (!hadStartBracket) {
      if (tok[0] == '(') {
        hadStartBracket = 1;
        movedBy += wcslen(tok);
        free((void *) tok);
        continue;

      } else {
        free((void *) tok);
        ts_token_syntax_error((wchar_t *) L"Function arguments starts before bracket", tsFile, token);
      }
    }

    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;
        free((void *) tok);
        break;
      }
      case L')': {
        movedBy += wcslen(tok);
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

        TSParserToken *arg = TS_parse_function_argument(tsFile, tsParseData);
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
  const wchar_t *tok;
  u_long movedBy = 0;
  volatile unsigned char proceed;
  volatile unsigned char foundColon = 0;

  proceed = 1;
  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      free((void *) tok);
      ts_token_syntax_error(
          (wchar_t *) L"Unexpected end of stream while looking for function return type",
          tsFile,
          token
      );
    }

    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;
        free((void *) tok);
        break;
      }
      case L':': {
        movedBy += wcslen(tok);
        free((void *) tok);
        proceed = 0;
        foundColon = 1;
        break;
      }
      case L'{': {
        movedBy += wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->character += movedBy;
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        ts_token_syntax_error(
            (wchar_t *) L"Unexpected token while looking for function type",
            tsFile,
            token
        );
      }
    }
  }

  if (foundColon == 1) {
    proceed = 1;
    while (proceed) {
      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        ts_token_syntax_error(
            (wchar_t *) L"Unexpected end of stream while looking for function return type",
            tsFile,
            token
        );
      }

      switch (tok[0]) {
        case L' ': {
          movedBy += wcslen(tok);
          free((void *) tok);
          break;
        }
        case L'\n': {
          tsParseData->line += 1;
          tsParseData->character = 0;
          tsParseData->position += movedBy;
          movedBy = 0;
          free((void *) tok);
          break;
        }
        case L'{': {
          if (data->returnType == NULL) {
            free((void *) tok);
            ts_token_syntax_error(
                (const wchar_t *) L"Found colon but type wasn't declared while parsing function!",
                tsFile,
                token
            );
          }
          proceed = 0;
          movedBy += wcslen(tok);
          free((void *) tok);
          break;
        }
        default: {
          if (data->returnType != NULL) {
            free((void *) tok);
            ts_token_syntax_error(
                (wchar_t *) L"Unexpected token while parsing function return type. Return type was already defined!",
                tsFile,
                token
            );
          }

          if (!TS_name_is_valid(tok)) {
            free((void *) tok);
            ts_token_syntax_error(
                (wchar_t *) L"Invalid type name for function return type!",
                tsFile,
                token
            );
          }

          data->returnType = TS_clone_string(tok);
          movedBy += wcslen(tok);
          free((void *) tok);
          break;
        }
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
    TSParserToken *token
) {
  const wchar_t *tok = NULL;
  u_long movedBy = 0;
  // move to bracket '{'
  volatile unsigned char proceed;

  proceed = 1;

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error(
          (wchar_t *) L"Unexpected end of function body",
          tsFile,
          token
      );
    }
    switch (tok[0]) {
      case L'\n': {
        movedBy += wcslen(tok);
        free((void *) tok);

        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;
        break;
      }
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'}': {
        movedBy += wcslen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        tsParseData->token = tok;

        TSParserToken *tsParserToken = TS_parse_ts_token(tsFile, tsParseData);

        movedBy += wcslen(tok);
        free((void *) tok);

        if (tsParserToken->tokenType != TS_UNKNOWN) {
          u_long size = token->childrenSize + 1;
          TSParserToken **newPointer = (TSParserToken **) calloc(sizeof(TSParserToken *), size);
          if (token->children) memcpy(newPointer, token->children, sizeof(TSParserToken *) * token->childrenSize);
          if (token->children) free(token->children);
          token->children = newPointer;
          token->children[token->childrenSize] = tsParserToken;
          token->childrenSize += 1;
        } else {
          TS_free_tsToken(tsParserToken);
        }
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
}

TSParserToken *
TS_parse_function(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("function");
  u_long movedBy = wcslen(tsParseData->token);

  TSFunctionData *functionData = calloc(sizeof(TSFunctionData), 1);
  functionData->arguments = NULL;
  functionData->argumentsSize = 0;
  functionData->name = NULL;
  functionData->returnType = NULL;

  TSParserToken *token = TS_build_parser_token(TS_FUNCTION, tsParseData);
  token->data = functionData;

  const wchar_t *tok;
  volatile unsigned char proceed = 1;

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error(
          (wchar_t *) L"Unexpected end of stream while parsing function",
          tsFile,
          token
      );
    }

    switch (tok[0]) {
      case L'\n': {
        movedBy += wcslen(tok);
        free((void *) tok);

        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;
        break;
      }
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'(': {
        if (token->name) {
          proceed = 0;
          TS_put_back(tsFile->stream, tok);
          free((void *) tok);
          break;
        }
        else if (token->parent) {
          proceed = 0;
          TS_put_back(tsFile->stream, tok);
          free((void *) tok);

          switch (token->parent->tokenType) {
            case TS_RETURN: {
              break;
            }
            case TS_CONST:
            case TS_LET:
            case TS_VAR: {
              token->name = TS_clone_string(token->parent->name);
              break;
            }
            default: {
              ts_token_syntax_error(
                  (const wchar_t *) L"Missing function name",
                  tsFile,
                  token
              );
            }
          }
        } else {
          free((void *) tok);
          ts_token_syntax_error(
              (const wchar_t *) L"Missing function name",
              tsFile,
              token
          );
        }
        break;
      }
      default: {
        if (TS_name_is_valid(tok)) {
          functionData->name = TS_clone_string(tok);
          free((void *) tok);
          proceed = 0;
        } else {
          free((void *) tok);
          ts_token_syntax_error(
              (wchar_t *) L"Invalid function name",
              tsFile,
              token
          );
        }
        break;
      }
    }
  }

  TS_parse_function_arguments(tsFile, tsParseData, token);
  TS_parse_function_lookup_return_type(tsFile, tsParseData, token);
  TS_parse_function_body(tsFile, tsParseData, token);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  tsParseData->parentTSToken = token->parent;

  TS_TOKEN_END("function");
  return token;
}

void
TS_free_function(
    TSParserToken *token
) {
  TS_free_children(token);

  TSFunctionData *data = token->functionData;
  free(token);
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
