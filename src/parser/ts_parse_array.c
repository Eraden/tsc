#include <cts/parser.h>

TSParserToken *
TS_parse_array(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_ARRAY, tsFile)

    wchar_t *tok = NULL;
    volatile unsigned char proceed = TRUE;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile);

      tok = (wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "array");
        break;
      }

      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L'\n': {
          TS_NEW_LINE(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L']': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        default: {
          tsFile->parse.token = tok;
          TSParserToken *child = TS_parse_ts_token(tsFile);
          free(tok);
          if (child) TS_push_child(token, child);
        }
      }
    }

  TS_TOKEN_END(TS_ARRAY)
}

void
TS_free_array(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}