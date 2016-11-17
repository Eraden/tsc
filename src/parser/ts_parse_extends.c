#include <cts/parser.h>
#include <cts/register.h>

TSParserToken *
TS_parse_extends(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_EXTENDS, tsFile)

    const wchar_t *tok;
    volatile unsigned char proceed = TRUE;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_get_token(tsFile->input.stream);
      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "class parent name");
        break;
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
          break;
        }
        default: {
          if (!TS_name_is_valid(tok)) {
            TS_token_syntax_error((wchar_t *) L"Invalid parent type name", tsFile, token);
          } else {
            TS_MOVE_BY(tsFile, tok);
            TSParserToken *typeToken = TS_find_type(tsFile->input.file, tok);
            if (typeToken == NULL) {
              TS_token_syntax_error((const wchar_t *) L"Unknown type used in extends", tsFile, token);
            } else {
              TS_push_child(token, TS_create_borrow(typeToken, tsFile));
            }
          }
          proceed = FALSE;
          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_EXTENDS);
}

void
TS_free_extends(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
