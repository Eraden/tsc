#include <cts/parser.h>

TSParserToken *TS_parse_break(TSFile *tsFile, TSParseData *tsParseData)
{
  TS_TOKEN_BEGIN("break");
  TS_MOVE_BY(tsParseData, tsParseData->token);
  const wchar_t *tok = (const wchar_t *) TS_getToken(tsFile->stream);
  free((void *) tok);

  TSParserToken *token = TS_build_parser_token(TS_BREAK, tsParseData);

  if (token->parent == NULL) {
    ts_token_syntax_error(
        (const wchar_t *) L"Unexpected break in global scope",
        tsFile,
        token
    );
  }

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("break");
  return token;
}

void TS_free_break(const TSParserToken *token)
{
  TS_free_unknown(token);
}