#include <cts/parser.h>

TSParserToken *
TS_parse_scope_or_json(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  wchar_t *buffer = NULL, *tok;
  volatile unsigned char proceed = TRUE;
  TSParseBracketType type = TS_PARSE_BRACKET_AS_SCOPE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (wchar_t *) TS_getToken(tsFile->stream);
    if (!tok) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsParseData->parentTSToken, "scope or json");
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
        wchar_t *extended = TS_join_strings(buffer, tok);
        if (buffer) free(buffer);
        free(tok);
        buffer = extended;
        proceed = FALSE;
        break;
      }
      default: {
        if (!TS_is_keyword(tok)) {
          type = TS_PARSE_BRACKET_AS_JSON;
        } else {
          type = TS_PARSE_BRACKET_AS_SCOPE;
        }
        proceed = FALSE;
        wchar_t *extended = TS_join_strings(buffer, tok);
        if (buffer) free(buffer);
        free(tok);
        buffer = extended;
        break;
      }
    }
  }
  if (buffer) {
    TS_put_back(tsFile->stream, buffer);
    free(buffer);
  }
  TSParserToken *token = NULL;
  switch (type) {
    case TS_PARSE_BRACKET_AS_SCOPE:
      token = TS_parse_scope(tsFile, tsParseData);
      break;
    case TS_PARSE_BRACKET_AS_JSON:
      token = TS_parse_json(tsFile, tsParseData);
      break;
  }
  return token;
}
