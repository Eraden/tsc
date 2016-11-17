#include <cts/parser.h>

TSParserToken *
TS_parse_inline_comment(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_INLINE_COMMENT, tsFile)

    token->modifiers = TS_MODIFIER_NONE;

    volatile unsigned char proceed = TRUE;
    const wchar_t *tok;
    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        break;
      }
      switch (tok[0]) {
        case L'\n': {
          TS_put_back(tsFile->input.stream, tok);
          proceed = FALSE;
          free((void *) tok);

          break;
        }
        default: {
          TS_MOVE_BY(tsFile, tok);
          u_long size = TS_STRING_END + wcslen(tok);
          if (token->content != NULL) size += wcslen(token->content);
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
          if (token->content != NULL) wcscpy(newPointer, token->name);
          if (token->content != NULL) free(token->content);
          wcscat(newPointer, tok);
          token->content = newPointer;
          free((void *) tok);
        }
      }
    }

  TS_TOKEN_END(TS_INLINE_COMMENT)
}

void
TS_free_inline_comment(
    const TSParserToken *token
) {
  if (token->content != NULL) free(token->content);
  free((void *) token);
}
