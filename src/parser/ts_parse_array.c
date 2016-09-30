#include <cts/parser.h>

TSParserToken *
TS_parse_array(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_MOVE_BY(tsParseData, tsParseData->token);
  TS_TOKEN_BEGIN("array");
  TSParserToken *token = TS_build_parser_token(TS_ARRAY, tsParseData);
  wchar_t *tok = NULL;
  volatile unsigned char proceed = TRUE;

  while (proceed) {
    tok = (wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "array");
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
      case L']': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
        free(tok);
        if (child) TS_push_child(token, child);
      }
    }
  }

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("array");
  return token;
}

void
TS_free_array(
    const TSParserToken *token
)
{
  TS_free_children(token);
  free((void *) token);
}