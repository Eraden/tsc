#include <cts/parser.h>

static void
__attribute__((visibility("hidden")))
TS_parse_local_variable_done(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSVariableParseFlag parseFlag
) {
  TSParserToken *token = tsParseData->parentTSToken;
  switch (parseFlag) {
    case TS_PARSE_VARIABLE_NAME: {
      ts_token_syntax_error(
          (wchar_t *) L"Missing variable name",
          tsFile,
          token
      );
      break;
    }
    case TS_PARSE_VARIABLE_TYPE: {
      ts_token_syntax_error(
          (wchar_t *) L"Expect variable type but none provided",
          tsFile,
          token
      );
      break;
    }
    case TS_PARSE_VARIABLE_VALUE: {
      ts_token_syntax_error(
          (wchar_t *) L"Expect variable default value but none provided",
          tsFile,
          token
      );
      break;
    }
    case TS_PARSE_VARIABLE_NONE: {}
    default: {
      break;
    }
  }
}

static void
__attribute__((visibility("hidden")))
TS_parse_local_variable_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = tsParseData->parentTSToken;
  const wchar_t *tok = NULL;
  volatile unsigned char proceed = TRUE;
  TSVariableParseFlag parseFlag = TS_PARSE_VARIABLE_NAME;
  TSLocalVariableData *data = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);

  data->name = NULL;
  data->value = NULL;
  data->type = NULL;
  token->variableData = data;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_parse_local_variable_done(tsFile, tsParseData, parseFlag);
      break;
    }
    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        free((void *) tok);
        ts_token_syntax_error(
            (wchar_t *) L"New line in variable definition is not allowed!",
            tsFile,
            token
        );
        proceed = FALSE;
        break;
      }
      case L':': {
        parseFlag = TS_PARSE_VARIABLE_TYPE;
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'=': {
        parseFlag = TS_PARSE_VARIABLE_VALUE;
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L';': {
        proceed = FALSE;
        TS_put_back(tsFile->stream, tok);
        free((void *) tok);
        break;
      }
      default: {
        if (wcscmp(tok, (const wchar_t *) L"in") == 0 || wcscmp(tok, (const wchar_t *) L"of") == 0) {
          TS_put_back(tsFile->stream, tok);
          TS_parse_local_variable_done(tsFile, tsParseData, parseFlag);
          proceed = FALSE;
          break;
        }
        if (parseFlag == TS_PARSE_VARIABLE_NAME) {
          log_to_file((wchar_t *) L"Setting name of local variable\n");
          if (TS_is_keyword(tok)) {
            free((void *) tok);
            ts_token_syntax_error(
                (const wchar_t *) L"Local variable name cannot use reserved word", tsFile, token
            );
            proceed = FALSE;
            break;
          }
          data->name = TS_clone_string(tok);
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          parseFlag = TS_PARSE_VARIABLE_NONE;
          log_to_file((wchar_t *) L"    Local variable current name: '%s'\n", data->name);

        } else if (parseFlag == TS_PARSE_VARIABLE_VALUE) {
          log_to_file((wchar_t *) L"%s", "Setting value for local variable\n");

          u_long size = TS_STRING_END + wcslen(tok);
          if (data->value) size += wcslen(data->value);
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
          if (data->value != NULL) wcscpy(newPointer, data->value);
          if (data->value != NULL) free((void *) data->value);
          wcscat(newPointer, tok);
          data->value = newPointer;
          log_to_file((wchar_t *) L"    Local variable current value: '%s'\n", data->value);

          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);

        } else if (parseFlag == TS_PARSE_VARIABLE_TYPE) {
          log_to_file(
              (wchar_t *) L"%s", "Setting type of local variable\n"
          );
          u_long size = TS_STRING_END + wcslen(tok);
          if (data->type) size += wcslen(data->type);
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
          if (data->type != NULL) wcscpy(newPointer, data->type);
          if (data->type != NULL) free((void *) data->type);
          wcscat(newPointer, tok);
          data->type = newPointer;
          log_to_file(
              (wchar_t *) L"    Local variable current type: '%s'\n",
              data->type
          );

          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);

        } else {
          free((void *) tok);
          ts_token_syntax_error(
              (wchar_t *) L"Unexpected token while parsing local variable\n",
              tsFile,
              token
          );
          proceed = FALSE;
        }
        break;
      }
    }
  }
}

TSParserToken *
TS_parse_var(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("var");
  TS_MOVE_BY(tsParseData, tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_VAR, tsParseData);
  TS_parse_local_variable_body(tsFile, tsParseData);

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("var");
  return token;
}

TSParserToken *
TS_parse_let(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("let");

  TS_MOVE_BY(tsParseData, tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_LET, tsParseData);
  TS_parse_local_variable_body(tsFile, tsParseData);

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("let");
  return token;
}

TSParserToken *
TS_parse_const(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("const");
  TS_MOVE_BY(tsParseData, tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_CONST, tsParseData);
  TS_parse_local_variable_body(tsFile, tsParseData);

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("const");
  return token;
}

void
TS_free_var(
    const TSParserToken *token
) {
  TS_free_children(token);

  TSLocalVariableData *data = token->variableData;
  free((void *) token);
  if (data == NULL) return;
  if (data->name != NULL) free((void *) data->name);
  if (data->type != NULL) free((void *) data->type);
  if (data->value != NULL) free((void *) data->value);
  free(data);
}

void
TS_free_let(
    const TSParserToken *token
) {
  TSLocalVariableData *data = token->variableData;
  free((void *) token);
  if (data == NULL) return;
  if (data->name != NULL) free((void *) data->name);
  if (data->type != NULL) free((void *) data->type);
  if (data->value != NULL) free((void *) data->value);
  free(data);
}

void
TS_free_const(
    const TSParserToken *token
) {
  TSLocalVariableData *data = token->variableData;
  free((void *) token);
  if (data == NULL) return;
  if (data->name != NULL) free((void *) data->name);
  if (data->type != NULL) free((void *) data->type);
  if (data->value != NULL) free((void *) data->value);
  free(data);
}
