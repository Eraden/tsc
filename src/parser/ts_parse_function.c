#include <cts/parser.h>

static void
__attribute__(( visibility("hidden")))
TS_parse_function_arguments(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = tsParseData->parentTSToken;
  const wchar_t *tok;
  u_short hadStartBracket = FALSE;
  volatile unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "function argument");
      break;
    }

    if (!hadStartBracket) {
      if (tok[0] == '(') {
        hadStartBracket = TRUE;
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        continue;

      } else {
        free((void *) tok);
        ts_token_syntax_error((wchar_t *) L"Function arguments starts before bracket", tsFile, token);
        break;
      }
    }

    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L')': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }

      default: {
        TS_put_back(tsFile->stream, tok);
        free((void *) tok);

        TSParserToken *arg = TS_parse_argument(tsFile, tsParseData);
        TS_push_child(token, arg);
        break;
      }
    }
  }
}

static void
__attribute((visibility("hidden")))
TS_parse_function_lookup_return_type(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = tsParseData->parentTSToken;
  TSFunctionData *data = token->data;
  const wchar_t *tok;
  volatile unsigned char proceed = TRUE;
  volatile unsigned char foundColon = FALSE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "function return type");
      break;
    }

    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L':': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        foundColon = TRUE;
        break;
      }
      case L'{': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, token, tok, "function type");
        proceed = FALSE;
      }
    }
  }

  if (foundColon == TRUE) {
    proceed = TRUE;
    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "function return type");
        break;
      }

      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          break;
        }
        case L'\n': {
          TS_NEW_LINE(tsParseData, tok);
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
          } else {
            TS_MOVE_BY(tsParseData, tok);
            free((void *) tok);
          }
          proceed = FALSE;
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
            proceed = FALSE;
          } else if (!TS_name_is_valid(tok)) {
            free((void *) tok);
            ts_token_syntax_error(
                (wchar_t *) L"Invalid type name for function return type!",
                tsFile,
                token
            );
            proceed = FALSE;
          } else {
            data->returnType = TS_clone_string(tok);
            TS_MOVE_BY(tsParseData, tok);
            free((void *) tok);
          }
          break;
        }
      }
    }
  }
}

static void
__attribute__((visibility("hidden")))
TS_parse_function_body(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token
) {
  const wchar_t *tok = NULL;
  // move to bracket '{'
  volatile unsigned char proceed;

  proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "function body");
      break;
    }
    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'}': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        tsParseData->token = tok;
        TS_MOVE_BY(tsParseData, tok);

        TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);

        free((void *) tok);

        if (child->tokenType != TS_UNKNOWN) {
          TS_push_child(token, child);
        } else {
          TS_free_tsToken(child);
        }
        break;
      }
    }
  }
}

TSParserToken *
TS_parse_function(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("function");
  TS_MOVE_BY(tsParseData, tsParseData->token);

  TSFunctionData *functionData = calloc(sizeof(TSFunctionData), 1);
  functionData->name = NULL;
  functionData->returnType = NULL;

  TSParserToken *token = TS_build_parser_token(TS_FUNCTION, tsParseData);
  token->data = functionData;

  const wchar_t *tok;
  volatile unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "function");
      break;
    }

    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'(': {
        if (token->name) {
          proceed = FALSE;
          TS_put_back(tsFile->stream, tok);
          free((void *) tok);
          break;
        } else if (token->parent) {
          proceed = FALSE;
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
              proceed = FALSE;
            }
          }
        } else {
          free((void *) tok);
          ts_token_syntax_error(
              (const wchar_t *) L"Missing function name",
              tsFile,
              token
          );
          proceed = FALSE;
        }
        break;
      }
      default: {
        if (TS_name_is_valid(tok)) {
          functionData->name = TS_clone_string(tok);
          free((void *) tok);
        } else {
          free((void *) tok);
          ts_token_syntax_error(
              (wchar_t *) L"Invalid function name",
              tsFile,
              token
          );
        }
        proceed = FALSE;
        break;
      }
    }
  }

  TS_parse_function_arguments(tsFile, tsParseData);
  TS_parse_function_lookup_return_type(tsFile, tsParseData);
  TS_parse_function_body(tsFile, tsParseData, token);

  tsParseData->parentTSToken = token->parent;

  TS_TOKEN_END("function");
  return token;
}

void
TS_free_function(
    const TSParserToken *token
) {
  TS_free_children(token);

  TSFunctionData *data = token->functionData;
  free((void *) token);
  if (data == NULL) return;
  if (data->name != NULL) free((void *) data->name);
  if (data->returnType != NULL) free((void *) data->returnType);
  free(data);
}
