#include <cts/parser.h>

static void
__attribute__(( visibility("hidden")))
TS_parse_else_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_log_to_file((wchar_t *) L"->   parsing as %s body\n", "else");
  const wchar_t *tok;
  TSParserToken *token = tsParseData->parentTSToken;

  unsigned char proceed;

  proceed = TRUE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "else body");
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

        return;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
        TS_push_child(token, child);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
    }
  }
  TS_log_to_file((wchar_t *) L"->   done %s body\n", "else");
}

TSParserToken *
TS_parse_else(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_ELSE, tsParseData)
    TS_parse_else_body(tsFile, tsParseData);
  TS_TOKEN_END(TS_ELSE);
}

void
TS_free_else(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
