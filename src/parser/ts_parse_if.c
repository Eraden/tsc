#include <cts/parser.h>

static void
TS_parse_if_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_log_to_file((wchar_t *) L"->   parsing as %s body\n", "if");
  TSParserToken *token = tsParseData->parentTSToken;
  const wchar_t *tok = NULL;

  volatile unsigned char proceed = TRUE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "`if` body (looking for brackets)");
      break;
    }

    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        free((void *) tok);
        break;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
        if (child) {
          TS_push_child(token, child);
        }
        proceed = FALSE;

        free((void *) tok);
        break;
      }
    }
  }
  TS_log_to_file((wchar_t *) L"->   done %s body\n", "if");
}

static void
TS_parse_if_conditions(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = tsParseData->parentTSToken;
  const wchar_t *tok;
  volatile unsigned char proceed = TRUE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of `if` conditions", tsFile, token);
      break;
    }

    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'(': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, token, tok, "`if` conditions");
        proceed = FALSE;
        break;
      }
    }
  }
  // after (

  proceed = TRUE;
  volatile unsigned long conditionsCount = 0;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "`if` conditions");
      break;
    }
    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L')': {
        if (conditionsCount == 0) {
          ts_token_syntax_error(
              (const wchar_t *) L"Unexpected end of `if` conditions. No condition given!",
              tsFile,
              token
          );
        } else {
          TS_MOVE_BY(tsParseData, tok);
        }
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        TS_MOVE_BY(tsParseData, tok);
        TSParserToken *child = TS_build_parser_token(TS_CONDITION, tsParseData);
        child->name = (void *) TS_clone_string(tok);
        TS_push_child(token, child);
        conditionsCount += 1;

        TS_MOVE_BY(tsParseData, tok);

        free((void *) tok);
        tsParseData->parentTSToken = child->parent;
        break;
      }
    }
  }
}

static void
TS_lookup_else(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = tsParseData->parentTSToken;
  const wchar_t *tok = NULL;
  volatile unsigned char proceed = TRUE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      return;
    }

    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        free((void *) tok);
        break;
      }
      default: {
        if (wcscmp(tok, (const wchar_t *) L"else") == 0) {
          TS_MOVE_BY(tsParseData, tok);
          tsParseData->token = tok;
          TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);

          TS_push_child(token, child);
        } else {
          TS_put_back(tsFile->stream, tok);
        }
        free((void *) tok);
        proceed = FALSE;
        break;
      }
    }
  }
}

TSParserToken *
TS_parse_if(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_IF, tsParseData)

    TS_MOVE_BY(tsParseData, tsParseData->token);
    token->data = NULL;

    TS_parse_if_conditions(tsFile, tsParseData);
    TS_parse_if_body(tsFile, tsParseData);
    TS_lookup_else(tsFile, tsParseData);

    tsParseData->parentTSToken = token->parent;

  TS_TOKEN_END(TS_IF)
}

void
TS_free_if(
    const TSParserToken *token
) {
  TS_free_children(token);

  free((void *) token);
}
