#include <cts/parser.h>

TSParserToken *
TS_parse_export(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_EXPORT, tsParseData)

    if (token->parent != NULL) {
      ts_token_syntax_error(
          (const wchar_t *) L"Unexpected parent for export. Export can be declared only in global scope.",
          tsFile, token
      );
    }

    volatile unsigned char proceed = TRUE;
    const wchar_t *tok = NULL;

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

  TS_TOKEN_END(TS_EXPORT)
}

void
TS_free_export(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
