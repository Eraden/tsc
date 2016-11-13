#include <cts/parser.h>

static TSParserToken *
TS_parse_default_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *scope = NULL;
  const wchar_t *tok = NULL;
  volatile unsigned char proceed = TRUE;
  TSParserToken *token = tsParseData->parentTSToken;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (const wchar_t *) TS_get_token(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "default");
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
      case L';': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
        if (child) {
          switch (child->tokenType) {
            case TS_SEMICOLON: {
              if (!scope) {
                scope = TS_build_parser_token(TS_SCOPE, tsParseData);
                tsParseData->parentTSToken = token;
                scope->parent = token;
              }
              TS_push_child(scope, child);
              proceed = FALSE;
              break;
            }
            case TS_CASE:
            case TS_DEFAULT: {
              TS_rollback_token(tsFile, tsParseData, child);
              proceed = FALSE;
              break;
            }
            case TS_SCOPE: {
              scope = child;
              proceed = FALSE;
              break;
            }
            default: {
              if (!scope) {
                scope = TS_build_parser_token(TS_SCOPE, tsParseData);
                tsParseData->parentTSToken = token;
                scope->parent = token;
              }
              TS_push_child(scope, child);
              break;
            }
          }
        } else {
          // todo error
        }
        free((void *) tok);
        break;
      }
    }
  }
  return scope;
}

TSParserToken *
TS_parse_default(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_DEFAULT, tsParseData)

    if (token->parent == NULL) {
      TS_token_syntax_error((const wchar_t *) L"Unexpected `default` without `switch` or `export`", tsFile, token);
    }

    const wchar_t *tok = NULL;
    volatile unsigned char proceed = TRUE;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile);

      tok = (const wchar_t *) TS_get_token(tsFile->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "default");
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
          break;
        }
        default: {
          TS_MOVE_BY(tsParseData, tok);
          if (token->parent->tokenType == TS_SWITCH) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "switch default. Default cannot have any condition!");
          } else if (token->parent->tokenType == TS_EXPORT) {
            tsParseData->token = tok;
            TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
            TS_push_child(token, child);
          }
          free((void *) tok);
          proceed = FALSE;
          break;
        }
      }
    }

    if (TS_is_embedded_in(token, TS_SWITCH)) {
      TSParserToken *scope = TS_parse_default_body(tsFile, tsParseData);
      if (scope) TS_push_child(token, scope);
    }

  TS_TOKEN_END(TS_DEFAULT);
}

void
TS_free_default(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
