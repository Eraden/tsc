#include <cts/register.h>

static void
TS_parse_do_scope(TSFile *tsFile) {
  wchar_t *tok = NULL;
  unsigned char proceed = TRUE;

  while (proceed) {
    tok = (wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsFile->parse.parentTSToken, "do body");
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
        TS_UNEXPECTED_TOKEN(tsFile, tsFile->parse.parentTSToken, tok, "do body");
        break;
      }
    }
    free((void *) tok);
  }
}

void TS_parse_do_while(TSFile *tsFile) {
  unsigned char proceed = TRUE;
  wchar_t *tok = NULL;

  while (proceed) {
    tok = (wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsFile->parse.parentTSToken, "do while");
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
      default: {
        tsFile->parse.token = tok;
        TSParserToken *conditions = TS_parse_while(tsFile);

        if (conditions) {
          switch (conditions->tokenType) {
            case TS_WHILE: {
              TS_push_child(tsFile->parse.parentTSToken, conditions);
              break;
            }
            default: {
              TS_UNEXPECTED_TOKEN(tsFile, conditions, "do while", "do")
              TS_free_ts_token(conditions);
              break;
            }
          }
        } else {
          // todo error
        }
        proceed = FALSE;
        break;
      }
    }
    free(tok);
  }
}

TSParserToken *
TS_parse_do(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_DO, tsFile);

    TS_parse_do_scope(tsFile);
    TS_parse_do_while(tsFile);

  TS_TOKEN_END(TS_do)
}

void
TS_free_do(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
