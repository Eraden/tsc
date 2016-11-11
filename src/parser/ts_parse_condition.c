#include <cts/parser.h>

TSParserToken *
TS_parse_condition(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_CONDITION, tsParseData)

    const wchar_t *tok = NULL;
    volatile unsigned char proceed = TRUE;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);
      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "condition");
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
        case L')': {
          TS_put_back(tsFile->stream, tok);
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        default: {
          TS_MOVE_BY(tsParseData, tok);

          tsParseData->token = tok;
          TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);

          if (child) {
            TS_push_child(token, child);
          } else {
            // TODO: error
          }

          proceed = FALSE;

          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_CONDITION)
}

void TS_free_condition(const TSParserToken *token) {
  TS_free_children(token);
  if (token->content) free(token->content);
  free((void *) token);
}

