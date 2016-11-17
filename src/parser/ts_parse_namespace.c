#include <cts/register.h>

TSParserToken *TS_parse_namespace(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_NAMESPACE, tsFile);

    wchar_t *tok = NULL;
    unsigned char proceed = TRUE;

    if (token->parent != NULL) {
      TS_token_syntax_error((const wchar_t *) L"Namespace must be in global scope!", tsFile, token);
      proceed = FALSE;
    }

    while (proceed) {
      tok = (wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "namespace");
        break;
      }

      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsFile, tok);
          break;
        }
        case L'\n': {
          TS_NEW_LINE(tsFile, tok);
          break;
        }
        case L'{': {
          if (token->name == NULL) {
            TS_MISSING_NAME(tsFile, token, "namespace");
          } else {
            proceed = FALSE;
            tsFile->parse.token = tok;
            TSParserToken *scope = TS_parse_scope(tsFile);
            if (scope) {
              TS_push_child(token, scope);
            } else {
              // todo error
            }
          }
          break;
        }
        default: {
          if (token->name == NULL && !TS_is_keyword(tok) && TS_name_is_valid(tok)) {
            token->name = TS_clone_string(tok);
          } else {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "namespace");
          }
          break;
        }
      }
      free((void *) tok);
    }

  TS_TOKEN_END(TS_NAMESPACE)
}

void TS_free_namespace(
    const TSParserToken *token
) {
  TS_free_children(token);
  if (token->name) free(token->name);
  free((void *) token);
}