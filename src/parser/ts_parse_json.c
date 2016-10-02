#include <cts/parser.h>

TSParserToken *
TS_parse_json(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_JSON, tsParseData)

    unsigned char proceed = TRUE;
    wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (wchar_t *) TS_getToken(tsFile->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "json");
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
          proceed = FALSE;
          break;
        }
        default: {
          // TODO implement
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_JSON)
}

void
TS_free_json(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
