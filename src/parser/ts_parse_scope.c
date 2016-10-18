#include <cts/parser.h>
#include <cts/register.h>

static void
TS_parse_scope_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = tsParseData->parentTSToken;
  TSParserToken *parent = token->parent;
  const wchar_t *tok = NULL;

  unsigned char IS_BODY;
  switch (parent ? parent->tokenType : TS_UNKNOWN) {
    case TS_FUNCTION:
    case TS_CLASS_METHOD: {
      IS_BODY = TRUE;
      break;
    }
    default: {
      IS_BODY = FALSE;
      break;
    }
  }

  while (1) {
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
        free((void *) tok);
        return;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
        switch (child->tokenType) {
          case TS_UNKNOWN: {
            TSParserToken *type = TS_search_token(child);
            if (type == NULL || type == child) {
              TS_free_tsToken(child);
            } else {
              TS_push_child(token, TS_create_borrow(type, tsParseData));
            }
            break;
          }
          case TS_RETURN: {
            if (IS_BODY) {
              TS_push_child(token, child);
            } else {
              TS_UNEXPECTED_TOKEN(tsFile, child, L"return", "scope");
            }
            break;
          }
          default: {
            TS_push_child(token, child);
          }
        }
        free((void *) tok);
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
