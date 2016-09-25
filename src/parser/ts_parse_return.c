#include <cts/parser.h>

TSParserToken *
TS_parse_return(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("return");
  TS_MOVE_BY(tsParseData, tsParseData->token);

  const wchar_t *tok;
  volatile unsigned char proceed = TRUE;

  TSParserToken *token = TS_build_parser_token(TS_RETURN, tsParseData);

  if (token->parent == NULL) {
    ts_token_syntax_error(
        (const wchar_t *) L"Return token in global scope",
        tsFile,
        token
    );
    proceed = FALSE;
  }
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
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
        proceed = FALSE;
        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);
        break;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
        TS_push_child(token, child);

        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("return");
  return token;
}

void
TS_free_return(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
