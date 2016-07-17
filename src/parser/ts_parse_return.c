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
        case ';':
        case '\n':
        {
          proceed = 0;
          movedBy += strlen(tok);
          free((void *) tok);
          break;
        }
        default:
        {
          tsParseData->token = tok;
          TSParserToken t = TS_parse_ts_token(tsFile, tsParseData);
          TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), token.childrenSize + 1);
          if (token.children != NULL) memcpy(newPointer, token.children, sizeof(TSParserToken) * token.childrenSize);
          if (token.children) free(token.children);
          token.children = newPointer;
          token.children[token.childrenSize] = t;
          token.childrenSize += 1;
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