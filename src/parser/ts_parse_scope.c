#include <tsc/parser.h>

static void TS_parse_scope_body(TSFile *tsFile, TSParseData *data, TSParserToken *token, u_long *movedBy) {
  const char *tok;
  while (1) {
    tok = (const char *) TS_getToken(data->stream);
    if (tok == NULL) {
      ts_syntax_error("Unexpected end of scope", tsFile->file, token->character, token->line);
    }
    switch (tok[0]) {
      case '}': {
        *movedBy += strlen(tok);
        free((void *) tok);
        return;
      }
      default: {
        data->token = tok;
        TSParserToken t = TS_parse_ts_token(tsFile, data);
        if (t.tokenType != TS_UNKNOWN) {
          TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), token->childrenSize + 1);
          if (token->children != NULL) memcpy(newPointer, token->children, sizeof(TSParserToken) * token->childrenSize);
          if (token->children != NULL) free(token->children);
          token->children = newPointer;
          token->children[token->childrenSize] = t;
          token->childrenSize += 1;
        } else {
          free(t.data);
        }
        free((void *) tok);
      }
    }
  }
}

const TSParserToken TS_parse_scope(TSFile *tsFile, TSParseData *tsParseData) {
  TS_TOKEN_BEGIN("scope");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_SCOPE;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.position = tsParseData->position;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  TS_parse_scope_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("scope");
  return token;
}

void TS_free_scope(const TSParserToken token) {
  // TODO
}
