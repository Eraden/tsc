#include <cts/register.h>

TSParserToken *
TS_parse_call_arguments(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_CALL_ARGUMENTS, tsFile)
    volatile unsigned char proceed = TRUE;
    const wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        if (token->data == NULL) {
          TS_token_syntax_error((wchar_t *) L"Unexpected end of stream while parsing `new` keyword.", tsFile, token);
          break;
        } else break;
      }

      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L'\n': {
          TS_NEW_LINE(tsFile, tok);
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        case L')': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        default: {
          TS_put_back(tsFile->input.stream, tok);
          tsFile->parse.token = tok;
          TSParserToken *child = TS_parse_argument(tsFile);
          if (child == NULL) {
            TS_token_syntax_error((const wchar_t *) L"Expecting call argument but nothing was found", tsFile, token);
          } else if (child->tokenType != TS_ARGUMENT) {
            TS_UNEXPECTED_TOKEN(tsFile, child, tok, "call arguments");
            TS_free_ts_token(child);
          } else {
            TS_push_child(token, child);
          }
          free((void *) tok);
          break;
        }
      }
    }
  TS_TOKEN_END(TS_CALL_ARGUMENTS)
}

void
TS_free_call_arguments(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}