#include <cts/parser.h>

TSParserToken *
TS_parse_case(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_CASE, tsFile)
    const wchar_t *tok = NULL;
    volatile unsigned char proceed = TRUE;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "case condition");
        break;
      }
      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L'\n': {
          TS_NEW_LINE(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L':': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);

          proceed = FALSE;
          break;
        }
        default: {
          TS_MOVE_BY(tsFile, tok);
          tsFile->parse.token = tok;

          if (token->childrenSize == 1) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "case condition. Only one is allowed!");
            proceed = FALSE;
          } else {
            TS_put_back(tsFile->input.stream, tok);
            tsFile->parse.token = (const wchar_t *) L"";

            TSParserToken *child = TS_parse_condition(tsFile);
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

      tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "case body");
        break;
      }

      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L'\n': {
          TS_NEW_LINE(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L'}': {
          if (!scope) {
            TS_put_back(tsFile->input.stream, tok);
            proceed = FALSE;
            free((void *) tok);
          } else {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "case body");
          }
          break;
        }
        default: {
          tsFile->parse.token = tok;
          TSParserToken *child = TS_parse_ts_token(tsFile);
          if (child) {
            switch (child->tokenType) {
              case TS_SEMICOLON: {
                if (!scope) {
                  scope = TS_build_parser_token(TS_SCOPE, tsFile);
                  tsFile->parse.parentTSToken = scope->parent;
                  TS_push_child(token, scope);
                }
                TS_push_child(scope, child);
                proceed = FALSE;
                break;
              }
              case TS_CASE:
              case TS_DEFAULT: {
                TS_rollback_token(tsFile, child);
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
                  scope = TS_build_parser_token(TS_SCOPE, tsFile);
                  tsFile->parse.parentTSToken = scope->parent;
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
