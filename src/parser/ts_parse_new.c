#include <cts/parser.h>

TSParserToken *
TS_parse_new(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("new");
  TS_MOVE_BY(tsParseData, tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_NEW, tsParseData);

  volatile unsigned char proceed = TRUE;
  const wchar_t *tok = NULL;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      if (token->data == NULL) {
        ts_token_syntax_error((wchar_t *) L"Unexpected end of stream while parsing `new` keyword.", tsFile, token);
        break;
      } else break;
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
          ts_token_syntax_error((wchar_t *) L"Expecting class after `new` keyword. Found new line.", tsFile, token);
          proceed = FALSE;
          break;
        } else {
          proceed = FALSE;
          TS_put_back(tsParseData->stream, tok);
          free((void *) tok);
        }
        break;
      }
      case L';': {
        if (token->childrenSize == 0) {
          free((void *) tok);
          ts_token_syntax_error((wchar_t *) L"Expecting class after `new` keyword. Found `;`.", tsFile, token);
          proceed = FALSE;
          break;
        } else {
          proceed = FALSE;
          TS_put_back(tsParseData->stream, tok);

          free((void *) tok);
        }
        break;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *caller = TS_parse_caller(tsFile, tsParseData);
        TS_push_child(token, caller);

        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->parentTSToken = token->parent;

  TS_TOKEN_END("new");
  return token;
}

void
TS_free_new(
    const TSParserToken *token
) {
  TS_free_children(token);

  if (token->data) free(token->data);
  free((void *) token);
}
