#include <tsc/parser.h>

const TSParserToken TS_parse_return(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "return");
  u_long movedBy = strlen(tsParseData->token);

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
    const char *tok;
    volatile unsigned char proceed = 1;
    while (proceed) {
      tok = (const char *) TS_getToken(tsParseData->stream);
      if (tok == NULL) {
        break;
      }
      switch (tok[0]) {
        case ' ': {
          movedBy += strlen(tok);
          free((void *) tok);
          break;
        }
        case '\n': {
          movedBy += strlen(tok);
          tsParseData->position += movedBy;
          tsParseData->line += 1;
          tsParseData->character = 0;
          movedBy = 0;
          break;
        }
        case ';':
        {
          proceed = 0;
          TS_put_back(tsParseData->stream, tok);
          break;
        }
        default:
        {
          tsParseData->token = tok;
          TSParserToken t = TS_parse_ts_token(tsFile, tsParseData);
          TS_push_child(&token, t);
          break;
        }
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  log_to_file("-> end %s\n", "return");
  return token;
}