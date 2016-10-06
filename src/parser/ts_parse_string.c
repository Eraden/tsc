#include <cts/parser.h>

TSParserToken *
TS_parse_string(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_STRING, tsParseData)

    const wchar_t beginToken = tsParseData->token[0];
    wchar_t *tok = NULL;
    unsigned char proceed = TRUE;
    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile);

      tok = (wchar_t *) TS_getToken(tsFile->stream);
      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "string");
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
          free(tok);
          break;
        }
        case L'\"':
        case L'\'': {
          TS_MOVE_BY(tsParseData, tok);
          if (tok[0] != beginToken) {
            wchar_t *newPointer = TS_join_strings(token->content, tok);
            if (token->content) free(token->content);
            token->content = newPointer;
          } else {
            proceed = FALSE;
          }
          free((void *) tok);
          break;
        }
        default: {
          TS_MOVE_BY(tsParseData, tok);
          wchar_t *newPointer = TS_join_strings(token->content, tok);
          if (token->content) free(token->content);
          token->content = newPointer;
          free(tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_STRING)
}

void
TS_free_string(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}