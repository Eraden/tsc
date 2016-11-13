#include <cts/parser.h>

TSParserToken *
TS_parse_case(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_CASE, tsParseData)
    const wchar_t *tok = NULL;
    volatile unsigned char proceed = TRUE;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsFile->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "case condition");
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
          tsParseData->token = tok;

          if (token->childrenSize == 1) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "case condition. Only one is allowed!");
            proceed = FALSE;
          } else {
            TS_put_back(tsFile->stream, tok);
            tsParseData->token = (const wchar_t *) L"";

            TSParserToken *child = TS_parse_condition(tsFile, tsParseData);
            if (child) {
              TS_push_child(token, child);
              proceed = FALSE;
            } else {
              // todo error
            }
          }
          free((void *) tok);
          break;
        }
      }
    }

    proceed = TRUE;

    TSParserToken *scope = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsFile->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "case body");
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
        default: {
          tsParseData->token = tok;
          TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
          if (child) {
            switch (child->tokenType) {
              case TS_SEMICOLON: {
                if (!scope) {
                  scope = TS_build_parser_token(TS_SCOPE, tsParseData);
                  tsParseData->parentTSToken = scope->parent;
                  TS_push_child(token, scope);
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
                TS_push_child(token, child);
                proceed = FALSE;
                break;
              }
              default: {
                if (!scope) {
                  scope = TS_build_parser_token(TS_SCOPE, tsParseData);
                  tsParseData->parentTSToken = scope->parent;
                  TS_push_child(token, scope);
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

  TS_TOKEN_END(TS_CASE);
}

void
TS_free_case(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
