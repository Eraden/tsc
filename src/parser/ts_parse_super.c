#include <cts/file.h>
#include <cts/parser.h>

wchar_t *TS_parse_super_method_name(TSFile *tsFile) {
  unsigned char proceed = TRUE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    wchar_t *tok = (wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsFile->parse.parentTSToken, "super method name");
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
      case L'.': {
        proceed = FALSE;
        break;
      }
      default: {
        return tok;
      }
    }
    free(tok);
  }
  return NULL;
}

TSParserToken *
TS_parse_super(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_SUPER, tsFile)
    token->childrenSize = 1;
    token->children = calloc(sizeof(TSParserToken *), 1);

    unsigned char proceed = TRUE;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      wchar_t *tok = (wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "super");
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
        case L'.': {
          wchar_t *methodName = TS_parse_super_method_name(tsFile);
          if (methodName) {
            token->name = TS_clone_string(methodName);
            free(methodName);
          } else {
            TS_MISSING_NAME(tsFile, token, "super method");
          }
          break;
        }
        case L'(': {
          tsFile->parse.token = tok;
          TSParserToken *callArgs = TS_parse_call_arguments(tsFile);
          token->children[0] = callArgs;
          proceed = FALSE;
          break;
        }
        default: {
          TS_UNEXPECTED_TOKEN(tsFile, token, tok, "super")
        }
      }
      free(tok);
    }
  TS_TOKEN_END(TS_SUPER)
}

void TS_free_super(const TSParserToken *token) {
  TS_free_children(token);
  if (token->name) free(token->name);
  free((void *) token);
}