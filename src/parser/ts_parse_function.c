#include <cts/parser.h>

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
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of function argument", tsFile, token);
      break;
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
        break;
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
        TS_put_back(tsFile->stream, tok);
        free((void *) tok);

        tsParseData->character += movedBy;
        tsParseData->position += movedBy;
        movedBy = 0;

        TSParserToken *arg = TS_parse_argument(tsFile, tsParseData);
        TS_push_child(token, arg);
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
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error(
          (wchar_t *) L"Unexpected end of stream while looking for function return type",
          tsFile,
          token
      );
      break;
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
        proceed = 0;
      }
    }
  }

  if (foundColon == 1) {
    proceed = 1;
    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        ts_token_syntax_error(
            (wchar_t *) L"Unexpected end of stream while looking for function return type",
            tsFile,
            token
        );
        break;
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
          } else {
            movedBy += wcslen(tok);
            free((void *) tok);
          }
          proceed = 0;
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
            proceed = 0;
          }
          else if (!TS_name_is_valid(tok)) {
            free((void *) tok);
            ts_token_syntax_error(
                (wchar_t *) L"Invalid type name for function return type!",
                tsFile,
                token
            );
            proceed = 0;
          } else {
            data->returnType = TS_clone_string(tok);
            movedBy += wcslen(tok);
            free((void *) tok);
          }
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
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error(
          (wchar_t *) L"Unexpected end of function body",
          tsFile,
          token
      );
      break;
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
  functionData->name = NULL;
  functionData->returnType = NULL;

  TSParserToken *token = TS_build_parser_token(TS_FUNCTION, tsParseData);
  token->data = functionData;

  const wchar_t *tok;
  volatile unsigned char proceed = 1;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error(
          (wchar_t *) L"Unexpected end of stream while parsing function",
          tsFile,
          token
      );
      break;
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
              proceed = 0;
            }
          }
        } else {
          free((void *) tok);
          ts_token_syntax_error(
              (const wchar_t *) L"Missing function name",
              tsFile,
              token
          );
          proceed = 0;
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
        proceed = 0;
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
