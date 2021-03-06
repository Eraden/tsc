#include "cts/register.h"

TSParserToken *TS_parse_group(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_GROUP, tsFile);
    unsigned char proceed = TRUE;
    const wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile);

      tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "group");
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
        case L')': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        default: {
          tsFile->parse.token = tok;
          TSParserToken *child = TS_parse_ts_token(tsFile);
          if (child) {
            TS_push_child(token, child);
          } else {
            // TODO: error
          }
          free((void *) tok);
          break;
        }
      }
    }
  TS_TOKEN_END(TS_GROUP)
}

void TS_free_group(const TSParserToken *token) {
  TS_free_children(token);
  free((void *) token);
}