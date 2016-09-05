#include <tsc/parser.h>

static void
TS_parse_scope_body(
    TSFile *tsFile,
    TSParseData *data,
    TSParserToken *token
) {
  const wchar_t *tok;
  u_long movedBy = 0;
  while (1) {
    tok = (const wchar_t *) TS_getToken(data->stream);
    if (tok == NULL) {
      ts_token_syntax_error((const wchar_t *) L"Unexpected end of scope", tsFile, token);
    }
    switch (tok[0]) {
      case L'}': {
        movedBy += wcslen(tok);
        free((void *) tok);

        data->position += movedBy;
        data->character += movedBy;
        return;
      }
      default: {
        data->token = tok;
        TSParserToken *t = TS_parse_ts_token(tsFile, data);
        if (t->tokenType != TS_UNKNOWN) {
          TSParserToken **newPointer = (TSParserToken **) calloc(sizeof(TSParserToken *), token->childrenSize + 1);
          if (token->children != NULL) memcpy(newPointer, token->children, sizeof(TSParserToken *) * token->childrenSize);
          if (token->children != NULL) free(token->children);
          token->children = newPointer;
          token->children[token->childrenSize] = t;
          token->childrenSize += 1;
        } else {
          TS_free_tsToken(token);
        }
        free((void *) tok);
      }
    }
  }
}

TSParserToken *
TS_parse_scope(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("scope");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken *token = TS_NEW_TOKEN;
  token->tokenType = TS_SCOPE;
  token->character = tsParseData->character;
  token->line = tsParseData->line;
  token->position = tsParseData->position;
  token->visibility = TS_VISIBILITY_SCOPE;
  token->children = NULL;
  token->childrenSize = 0;
  token->data = NULL;

  TS_parse_scope_body(tsFile, tsParseData, token);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("scope");
  return token;
}

void
TS_free_scope(
    const TSParserToken *token
) {
  free((void *) token);
}
