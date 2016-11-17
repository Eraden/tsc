#include <cts/register.h>

static void
TS_parse_if_conditions(TSFile *tsFile) {
  const wchar_t *tok = NULL;
  volatile unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsFile->parse.parentTSToken, "`if` conditions");
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
        TS_MOVE_BY(tsFile, tok);
        tsFile->parse.token = tok;
        TSParserToken *condition = TS_parse_condition(tsFile);
        TS_push_child(tsFile->parse.parentTSToken, condition);
        proceed = FALSE;
        free((void *) tok);
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, tsFile->parse.parentTSToken, tok, "`if` conditions");
        proceed = FALSE;
        break;
      }
    }
  }
}

static void
TS_lookup_else(TSFile *tsFile) {
  TSParserToken *token = tsFile->parse.parentTSToken;
  const wchar_t *tok = NULL;
  volatile unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      return;
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
        if (wcscmp(tok, (const wchar_t *) L"else") == 0) {
          TS_MOVE_BY(tsFile, tok);
          tsFile->parse.token = tok;
          TSParserToken *child = TS_parse_ts_token(tsFile);

          TS_push_child(token, child);
        } else {
          TS_put_back(tsFile->input.stream, tok);
        }
        free((void *) tok);
        proceed = FALSE;
        break;
      }
    }
  }
}

static void
TS_parse_if_body(TSFile *tsFile) {
  TS_log_to_file((wchar_t *) L"->   parsing as %s body\n", "if");
  TSParserToken *token = tsFile->parse.parentTSToken;
  const wchar_t *tok = NULL;
  TSParserToken *scope = NULL;

  volatile unsigned char proceed = TRUE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "`if` body (looking for brackets)");
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
        tsFile->parse.token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile);
        if (child) {
          switch (child->tokenType) {
            case TS_SEMICOLON: {
              if (!scope) {
                scope = TS_build_parser_token(TS_SCOPE, tsFile);
                tsFile->parse.parentTSToken = token;
                TS_push_child(token, scope);
              }
              child->parent = scope;
              TS_push_child(scope, child);
              TS_lookup_else(tsFile);
              proceed = FALSE;
              break;
            }
            case TS_SCOPE: {
              scope = child;
              TS_push_child(token, scope);
              TS_lookup_else(tsFile);
              proceed = FALSE;
              break;
            }
            case TS_ELSE: {
              TS_push_child(token, child);
              proceed = FALSE;
              break;
            }
            default: {
              if (!scope) {
                scope = TS_build_parser_token(TS_SCOPE, tsFile);
                tsFile->parse.parentTSToken = token;
                child->parent = scope;
                TS_push_child(token, scope);
                TS_push_child(scope, child);
              } else {
                TS_rollback_token(tsFile, child);
                proceed = FALSE;
              }

              break;
            }
          }

        }

        free((void *) tok);
        break;
      }
    }
  }
  TS_log_to_file((wchar_t *) L"->   done %s body\n", "if");
}

TSParserToken *
TS_parse_if(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_IF, tsFile)

    TS_MOVE_BY(tsFile, tsFile->parse.token);
    token->data = NULL;

    TS_parse_if_conditions(tsFile);
    TS_parse_if_body(tsFile);

    tsFile->parse.parentTSToken = token->parent;

  TS_TOKEN_END(TS_IF)
}

void
TS_free_if(
    const TSParserToken *token
) {
  TS_free_children(token);

  free((void *) token);
}
