#import <cts/parser.h>

/**
 * TODO implement
 */
TSParserToken *
TS_parse_export(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("export");
  TSParserToken *token = TS_build_parser_token(TS_EXPORT, tsParseData);

  volatile unsigned char proceed = TRUE;
  const wchar_t *tok;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      if (token->childrenSize == 1)
        break;
      else {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "`export` keyword");
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
        if (token->childrenSize == 0) {
          free((void *) tok);
          ts_token_syntax_error(
              (wchar_t *) L"Expecting expression after `export` keyword. Found new line.",
              tsFile,
              token
          );
          break;
        } else {
          proceed = FALSE;
          TS_put_back(tsParseData->stream, tok);
          free((void *) tok);
          break;
        }
      }
      case L';': {
        proceed = FALSE;
        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);
        break;
      }
      default: {
        if (token->childrenSize == 1) {
          TS_put_back(tsParseData->stream, tok);
          proceed = FALSE;
        } else {
          TS_MOVE_BY(tsParseData, tok);

          tsParseData->token = tok;
          TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);

          TS_push_child(token, child);
        }
        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("export");
  return token;
}

void
TS_free_export(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
