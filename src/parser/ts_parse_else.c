#include <cts/parser.h>

static void
TS_parse_else_body(TSFile *tsFile) {
  TS_log_to_file((wchar_t *) L"->   parsing as %s body\n", "else");
  const wchar_t *tok;
  TSParserToken *token = tsFile->parse.parentTSToken;

  unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "else body");
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
      case L';': {
        TS_MOVE_BY(tsFile, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        tsFile->parse.token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile);
        TS_push_child(token, child);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
    }
  }
  TS_log_to_file((wchar_t *) L"->   done %s body\n", "else");
}

TSParserToken *
TS_parse_else(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_ELSE, tsFile)
    if (!token->parent || token->parent->tokenType != TS_IF) {
      TS_token_syntax_error((const wchar_t *) L"Missing `if` for `else`", tsFile, token);
    } else {
      TS_parse_else_body(tsFile);
    }
  TS_TOKEN_END(TS_ELSE);
}

void
TS_free_else(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
