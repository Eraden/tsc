#include <cts/parser.h>

TSParserToken *
TS_parse_return(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_RETURN, tsFile)

    const wchar_t *tok = NULL;
    volatile unsigned char proceed = TRUE;

    if (!(TS_is_embedded_in(token, TS_CLASS_METHOD) || TS_is_embedded_in(token, TS_FUNCTION))) {
      TS_UNEXPECTED_GLOBAL_TOKEN(tsFile, token, "return")
    }

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
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
        case L';': {
          proceed = FALSE;
          TS_put_back(tsFile->input.stream, tok);
          free((void *) tok);
          break;
        }
        default: {
          tsFile->parse.token = tok;
          TSParserToken *child = TS_parse_ts_token(tsFile);
          TS_push_child(token, child);

          free((void *) tok);
          // FIXME: multiple return!!!
          break;
        }
      }
    }

  TS_TOKEN_END(TS_RETURN)
}

void
TS_free_return(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
