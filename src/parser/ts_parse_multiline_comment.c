#include <tsc/parser.h>

const
TSParserToken TS_parse_multiline_comment(
    TSFile __attribute__((__unused__)) *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("Multiline token");

  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_MULTILINE_COMMENT;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_NONE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  volatile unsigned char proceed = 1;
  const char *tok;
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of multiline comment", tsFile, &token);
    }
    if (strlen(tok) == 2 && tok[0] == '*' && tok[1] == '/') {
      proceed = 0;
      movedBy += strlen(tok);
      free((void *) tok);
    } else {
      u_long size = TS_STRING_END + strlen(tok);
      if (token.data != NULL) size += strlen(token.data);
      char *newPointer = (char *) calloc(sizeof(char), size);
      if (token.data != NULL) strcpy(newPointer, token.data);
      if (token.data != NULL) free(token.data);
      strcat(newPointer, tok);
      token.data = newPointer;
      movedBy += strlen(tok);
      free((void *) tok);
    }
  }

  TS_TOKEN_END("Multiline token")

  return token;
}

void TS_free_multiline_comment(const TSParserToken token) {
  if (token.data != NULL) free(token.data);
}
