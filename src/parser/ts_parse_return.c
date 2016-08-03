#include <tsc/parser.h>

const TSParserToken TS_parse_return(TSFile *tsFile, TSParseData *tsParseData) {
  TS_TOKEN_BEGIN("return");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_RETURN;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  {
    const wchar_t *tok;
    volatile unsigned char proceed = 1;
    while (proceed) {
      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        break;
      }

      switch (tok[0]) {
        case L' ': {
          movedBy += wcslen(tok);
          free((void *) tok);
          break;
        }
        case L'\n': {
          movedBy += wcslen(tok);
          tsParseData->position += movedBy;
          tsParseData->line += 1;
          tsParseData->character = 0;
          movedBy = 0;

          free((void *) tok);
          break;
        }
        case L';':
        {
          proceed = 0;
          TS_put_back(tsParseData->stream, tok);
          free((void *) tok);
          break;
        }
        default:
        {
          tsParseData->token = tok;
          TSParserToken child = TS_parse_ts_token(tsFile, tsParseData);
          TS_push_child(&token, child);

          free((void *) tok);
          break;
        }
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  TS_TOKEN_END("return");
  return token;
}

void TS_free_return(const TSParserToken token) {
  TS_free_children(token);
}
