#include <cts/parser.h>

TSParserToken *
TS_parse_extends(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_EXTENDS, tsParseData)

    const wchar_t *tok;
    volatile unsigned char proceed = TRUE;
    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);
      if (tok == NULL) {
        ts_token_syntax_error(
            (wchar_t *) L"Unexpected end of stream while parsing class parent name",
            tsFile,
            token
        );
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
        default: {
          if (!TS_name_is_valid(tok)) {
            free((void *) tok);
            ts_token_syntax_error(
                (wchar_t *) L"Invalid parent class name",
                tsFile,
                token
            );
          } else {
            TS_MOVE_BY(tsParseData, tok);
            token->name = TS_clone_string(tok);
          }
          proceed = FALSE;
          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_EXTENDS);
}

void
TS_free_extends(
    const TSParserToken *token
) {
  if (token->name) free(token->name);
  free((void *) token);
}
