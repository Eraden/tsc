#include <cts/register.h>

static void
TS_parse_while_conditions(TSFile *tsFile) {
  wchar_t *tok = NULL;
  unsigned char proceed = TRUE;

  while (proceed) {
    tok = (wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsFile->parse.parentTSToken, "while conditions");
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
      case L'(': {
        tsFile->parse.token = tok;
        TSParserToken *condition = TS_parse_condition(tsFile);
        if (condition) {
          TS_push_child(tsFile->parse.parentTSToken, condition);
        } else {
          // todo error
        }
        proceed = FALSE;
        free((void *) tok);
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, tsFile->parse.parentTSToken, tok, "while conditions");
        break;
      }
    }
  }
}

static void
TS_parse_while_scope(TSFile *tsFile) {
  wchar_t *tok = NULL;
  unsigned char proceed = TRUE;

  while (proceed) {
    tok = (wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsFile->parse.parentTSToken, "while body");
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
        tsFile->parse.token = tok;
        TSParserToken *condition = TS_parse_scope(tsFile);
        if (condition) {
          TS_push_child(tsFile->parse.parentTSToken, condition);
        } else {
          // todo error
        }
        proceed = FALSE;
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, tsFile->parse.parentTSToken, tok, "while body");
        break;
      }
    }
    free((void *) tok);
  }
}

TSParserToken *
TS_parse_while(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_WHILE, tsFile)

    TS_parse_while_conditions(tsFile);
    if (token->parent == NULL || token->parent->tokenType != TS_DO) {
      TS_parse_while_scope(tsFile);
    }

  TS_TOKEN_END(TS_WHILE)
}

void
TS_free_while(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}