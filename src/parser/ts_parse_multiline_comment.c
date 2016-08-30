#include <tsc/parser.h>

const
TSParserToken TS_parse_multiline_comment(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("Multiline token");

  u_long movedBy = wcslen(tsParseData->token);

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
  const wchar_t *tok;
  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of multiline comment", tsFile, &token);
    }
    if (wcslen(tok) == 2 && tok[0] == L'*' && tok[1] == L'/') {
      proceed = 0;
      movedBy += wcslen(tok);
      free((void *) tok);
    } else {
      u_long size = TS_STRING_END + wcslen(tok);
      if (token.data != NULL) size += wcslen(token.data);
      wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
      if (token.data != NULL) wcscpy(newPointer, token.data);
      if (token.data != NULL) free(token.data);
      wcscat(newPointer, tok);
      token.data = newPointer;
      movedBy += wcslen(tok);
      free((void *) tok);
    }
  }

  TS_TOKEN_END("Multiline token")

  return token;
}

void TS_free_multiline_comment(const TSParserToken token) {
  if (token.data != NULL) free(token.data);
}
