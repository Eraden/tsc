#include <cts/register.h>
#include <cts/file.h>

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
          TSParserToken *child = NULL;
          if (wcscmp(tok, (const wchar_t *) L"...") == 0) {
            tsFile->parse.token = tok;
            child = TS_parse_spread(tsFile);
          } else if (wcscmp(tok, (const wchar_t *) L"{") == 0) {
            tsFile->parse.token = tok;
            child = TS_parse_json(tsFile);
          } else if (TS_name_is_valid(tok) && !TS_is_keyword(tok)) {
            TS_put_back(tsFile->input.stream, tok);
            tsFile->parse.token = tok;
            child = TS_parse_argument(tsFile);
          } else {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "call arguments");
          }

          if (child) {
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