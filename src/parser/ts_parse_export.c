#import <tsc/parser.h>

/**
 * TODO implement
 */
const TSParserToken TS_parse_export(TSFile *__attribute__((__unused__)) tsFile, TSParseData *tsParseData) {
  TS_TOKEN_BEGIN("export");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_EXPORT;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  volatile unsigned char proceed = 1;
  const char *tok;
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
        if (token.childrenSize == 0) {
          ts_token_syntax_error("Expecting expression after `export` keyword. Found new line.", tsFile, &token);
        } else {
          proceed = 0;
          TS_put_back(tsParseData->stream, tok);
          free((void *) tok);
          break;
        }
      }
      case ';': {
        proceed = 0;
        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);
        break;
      }
      default: {
        if (token.childrenSize == 1) {
          TS_put_back(tsParseData->stream, tok);
          proceed = 0;
          break;
        }

        tsParseData->token = tok;
        tsParseData->character += movedBy;
        tsParseData->position += movedBy;
        movedBy = 0;
        TSParserToken child = TS_parse_ts_token(tsFile, tsParseData);

        TS_push_child(&token, child);

        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  TS_TOKEN_END("export");
  return token;
}

void TS_free_export(const TSParserToken token) {
  TS_free_children(token);
}
