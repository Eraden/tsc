#include <tsc/parser.h>

TSParserToken *
TS_parse_inline_comment(
    TSFile __attribute__((__unused__)) *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("Inline token");

  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_INLINE_COMMENT, tsParseData);
  token->visibility = TS_VISIBILITY_NONE;

  volatile unsigned char proceed = 1;
  const wchar_t *tok;
  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      break;
    }
    switch (tok[0]) {
      case L'\n': {
        TS_put_back(tsParseData->stream, tok);
        proceed = 0;
        free((void *) tok);

        break;
      }
      default: {
        u_long size = TS_STRING_END + wcslen(tok);
        if (token->content != NULL) size += wcslen(token->content);
        wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
        if (token->content != NULL) wcscpy(newPointer, token->name);
        if (token->content != NULL) free(token->content);
        wcscat(newPointer, tok);
        token->content = newPointer;
        movedBy += wcslen(tok);
        free((void *) tok);
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  tsParseData->parentTSToken= token->parent;
  TS_TOKEN_END("Inline token")

  return token;
}

void
TS_free_inline_comment(
    const TSParserToken *token
) {
  if (token->content != NULL) free(token->content);
  free((void *) token);
}
