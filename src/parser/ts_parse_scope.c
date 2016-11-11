#include <cts/parser.h>
#include <cts/register.h>

static void
TS_parse_scope_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = tsParseData->parentTSToken;
  const wchar_t *tok = NULL;
  volatile unsigned char proceed = TRUE;

  unsigned char IS_BODY = (unsigned char) (TS_isEmbeddedIn(token, TS_CLASS_METHOD) || TS_isEmbeddedIn(token, TS_FUNCTION));

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "scope");
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
        proceed = FALSE;
        free((void *) tok);
        break;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);

        switch (child->tokenType) {
          case TS_UNKNOWN: {
            TSParserToken *resolved = TS_search_token(child);

            if (resolved == child) {
              TS_push_child(token, child);
            } else if (resolved) {
              TS_push_child(token, TS_create_borrow(resolved, tsParseData));
              // Replaced with new borrow
              TS_free_tsToken(child);
            } else {
              // TODO:
              // Temporary ignore missing definition for:
              // instance.call();
              // `.` needs to be implemented!
              // TS_UNEXPECTED_TOKEN(tsFile, child, tok, "scope");
              TS_free_tsToken(child);
            }
            break;
          }
          case TS_RETURN: {
            if (IS_BODY) {
              TS_push_child(token, child);
            } else {
              TS_UNEXPECTED_TOKEN(tsFile, child, L"return", "scope");
              TS_free_tsToken(child);
            }
            break;
          }
          default: {
            TS_push_child(token, child);
            break;
          }
        }
        free((void *) tok);
        break;
      }
    }
  }
}

TSParserToken *
TS_parse_scope(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_SCOPE, tsParseData)

    TS_parse_scope_body(tsFile, tsParseData);

  TS_TOKEN_END(TS_SCOPE)
}

void
TS_free_scope(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
