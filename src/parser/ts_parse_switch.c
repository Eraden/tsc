#include <cts/parser.h>

static TSParserToken *
TS_parse_switch_conditions(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_SECTION_BEGIN("switch conditions");
  TSParserToken *token = tsParseData->parentTSToken;
  TSParserToken *child = NULL;

  const wchar_t *tok;
  volatile unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "switch conditions section");
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
        TS_put_back(tsFile->stream, tok);
        tsParseData->token = tok;
        child = TS_parse_condition(tsFile, tsParseData);
        proceed = FALSE;
        free((void *) tok);
        break;
      }
    }
  }

  TS_TOKEN_SECTION_END("switch conditions");
  return child;
}

static void
TS_parse_switch_skip_to_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_SECTION_BEGIN("switch skipping to body");
  TSParserToken *token = tsParseData->parentTSToken;
  volatile unsigned char proceed = TRUE;
  const wchar_t *tok = NULL;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsFile->stream);
    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "switch");
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
      case L'{': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, token, tok, "switch");
        free((void *) tok);
        proceed = FALSE;
        break;
      }
    }
  }
  TS_TOKEN_SECTION_END("switch skipping to body")
}

static TSParserToken *
TS_parse_switch_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_SECTION_BEGIN("switch body");
  TSParserToken *token = tsParseData->parentTSToken;
  TSParserToken *child = NULL;

  const wchar_t *tok;
  volatile unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "switch body");
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
      case L'}': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        tsParseData->token = tok;
        child = TS_parse_ts_token(tsFile, tsParseData);

        switch (child->tokenType) {
          case TS_CASE:
          case TS_DEFAULT: {
            break;
          }
          default: {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "switch body");
            TS_free_tsToken(child);
            child = NULL;
            break;
          }
        }

        free((void *) tok);
        proceed = FALSE;
        break;
      } // default
    } // switch

  } // while
  TS_TOKEN_SECTION_END("switch body");

  return child;
}


TSParserToken *
TS_parse_switch(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_SWITCH, tsParseData)

    TSParserToken *child = NULL;

    volatile long brackets_count = FALSE;

    while (1) {
      TS_LOOP_SANITY_CHECK(tsFile)

      child = TS_parse_switch_conditions(tsFile, tsParseData);
      if (child == NULL) break;

      if (wcscmp(child->name, (const wchar_t *) L"(") == 0) {
        brackets_count += 1;
        if (brackets_count > 1) TS_push_child(token, child);
        else TS_free_tsToken(child);
      } else if (wcscmp(child->name, (const wchar_t *) L")") == 0) {
        brackets_count -= 1;
        if (brackets_count > 0) {
          TS_push_child(token, child);
        }

        if (brackets_count == 0) {
          TS_free_tsToken(child);
          break;
        } else if (brackets_count < 0) {
          ts_token_syntax_error(
              (const wchar_t *) L"Unexpected token while parsing switch conditions",
              tsFile,
              child
          );
          break;
        }
      } else {
        TS_push_child(token, child);

        if (brackets_count <= 0) {
          ts_token_syntax_error(
              (const wchar_t *) L"Unexpected token while parsing switch conditions",
              tsFile,
              child
          );
          break;
        }
      }
    }

    TS_parse_switch_skip_to_body(tsFile, tsParseData);

    while (1) {
      TS_LOOP_SANITY_CHECK(tsFile)

      child = TS_parse_switch_body(tsFile, tsParseData);
      if (child == NULL) break;
      else TS_push_child(token, child);
    }

  TS_TOKEN_END(TS_SWITCH)
}

void
TS_free_switch(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
