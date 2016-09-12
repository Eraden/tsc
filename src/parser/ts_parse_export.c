#import <tsc/parser.h>

/**
 * TODO implement
 */
TSParserToken *
TS_parse_export(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("export");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_EXPORT, tsParseData);

  volatile unsigned char proceed = 1;
  const wchar_t *tok;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      if (token->childrenSize == 1)
        break;
      else
        ts_token_syntax_error((wchar_t *) L"Unexpected end of stream while parsing `export` keyword.", tsFile, token);
    }

    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        if (token->childrenSize == 0) {
          ts_token_syntax_error((wchar_t *) L"Expecting expression after `export` keyword. Found new line.", tsFile, token);
        } else {
          proceed = 0;
          TS_put_back(tsParseData->stream, tok);
          free((void *) tok);
          break;
        }
      }
      case L';': {
        proceed = 0;
        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);
        break;
      }
      default: {
        if (token->childrenSize == 1) {
          TS_put_back(tsParseData->stream, tok);
          proceed = 0;
        } else {
          movedBy += wcslen(tok);

          tsParseData->token = tok;
          tsParseData->character += movedBy;
          tsParseData->position += movedBy;
          movedBy = 0;
          TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);

          TS_push_child(token, child);
        }
        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
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
