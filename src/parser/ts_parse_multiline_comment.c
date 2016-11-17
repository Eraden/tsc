#include <cts/parser.h>

TSParserToken *
TS_parse_multiline_comment(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_MULTILINE_COMMENT, tsFile)

    token->modifiers = TS_MODIFIER_NONE;

    volatile unsigned char proceed = TRUE;
    const wchar_t *tok;
    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_get_token(tsFile->input.stream);
      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "multiline comment");
        break;
      }
      if (wcslen(tok) == 2 && tok[0] == L'*' && tok[1] == L'/') {
        TS_MOVE_BY(tsFile, tok);
        free((void *) tok);
        proceed = FALSE;
      } else {
        u_long size = TS_STRING_END + wcslen(tok);
        if (token->data != NULL) size += wcslen(token->data);
        wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
        if (token->data != NULL) wcscpy(newPointer, token->data);
        if (token->data != NULL) free(token->data);
        wcscat(newPointer, tok);
        token->data = newPointer;
        TS_MOVE_BY(tsFile, tok);
        free((void *) tok);
      }
    }

  TS_TOKEN_END(TS_MULTILINE_COMMENT)
}

void
TS_free_multiline_comment(
    const TSParserToken *token
) {
  if (token->data != NULL) free(token->data);
  free((void *) token);
}
