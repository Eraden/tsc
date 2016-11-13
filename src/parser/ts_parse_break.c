#include <cts/parser.h>

TSParserToken *TS_parse_break(TSFile *tsFile, TSParseData *tsParseData) {
  TS_TOKEN_BEGIN(TS_BREAK, tsParseData)
//    const wchar_t *tok = (const wchar_t *) TS_get_token(tsFile->stream);
//    free((void *) tok);

    if (token->parent == NULL) {
      TS_UNEXPECTED_GLOBAL_TOKEN(tsFile, token, "break");
    }

  TS_TOKEN_END(TS_BREAK)
}

void TS_free_break(const TSParserToken *token) {
  TS_free_unknown(token);
}