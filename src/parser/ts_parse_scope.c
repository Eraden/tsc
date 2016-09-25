#include <cts/parser.h>

static void
TS_parse_scope_body(
    TSFile *tsFile,
    TSParseData *data,
    TSParserToken *token
) {
  const wchar_t *tok;
  while (1) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(data->stream);
    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "scope");
      break;
    }
    switch (tok[0]) {
      case L'\n': {
        u_long movedBy = wcslen(tok);
        data->position += movedBy;
        data->line += 1;
        data->character = 0;
        free((void *) tok);
        break;
      }
      case L' ': {
        u_long len = wcslen(tok);
        data->character += len;
        data->position += len;
        free((void *) tok);
        break;
      }
      case L'}': {
        u_long movedBy = wcslen(tok);
        free((void *) tok);

        data->position += movedBy;
        data->character += movedBy;
        return;
      }
      default: {
        data->token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile, data);
        if (child->tokenType != TS_UNKNOWN) {
          TS_push_child(token, child);
        } else {
          TS_free_tsToken(child);
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
  TS_TOKEN_BEGIN("scope");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_SCOPE, tsParseData);

  TS_parse_scope_body(tsFile, tsParseData, token);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("scope");
  return token;
}

void
TS_free_scope(
    const TSParserToken *token
) {
  free((void *) token);
}
