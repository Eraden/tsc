#include <tsc/parser.h>

static TSParserToken *
TS_parse_switch_conditions(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_SECTION_BEGIN("switch conditions");
  TSParserToken *token = tsParseData->parentTSToken;
  TSParserToken *child = NULL;

  const wchar_t *tok;
  volatile unsigned char proceed = 1;

  while(proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      ts_token_syntax_error(
          (const wchar_t *) L"Unexpected end of stream while parsing switch conditions section",
          tsFile,
          token
      );
      break;
    }
    switch (tok[0]) {
      case L' ': {
        u_long movedBy = wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->character += movedBy;
        free((void *) tok);
        break;
      }
      case L'\n': {
        u_long movedBy = wcslen(tok);
        free((void *) tok);
        tsParseData->position += movedBy;
        tsParseData->character = 0;
        tsParseData->line += 1;
        break;
      }
      default: {
        TS_put_back(tsFile->stream, tok);
        tsParseData->token = tok;
        child = TS_parse_condition(tsFile, tsParseData);
        proceed = 0;
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
)
{
  TS_TOKEN_SECTION_BEGIN("switch skipping to body");
  TSParserToken *token = tsParseData->parentTSToken;
  volatile unsigned char proceed = 1;
  const wchar_t *tok;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsFile->stream);
    if (tok == NULL) {
      ts_token_syntax_error(
          (const wchar_t *) L"Unexpected end of stream while parsing switch",
          tsFile,
          token
      );
      break;
    }
    switch (tok[0]) {
      case L' ': {
        u_long movedBy = wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->character += movedBy;
        free((void *) tok);
        break;
      }
      case L'\n': {
        u_long movedBy = wcslen(tok);
        free((void *) tok);
        tsParseData->position += movedBy;
        tsParseData->character = 0;
        tsParseData->line += 1;
        break;
      }
      case L'{': {
        u_long movedBy = wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->character += movedBy;
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        free((void *) tok);
        ts_token_syntax_error(
            (const wchar_t *) L"Unexpected token while parsing switch",
            tsFile,
            token
        );
        proceed = 0;
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
  volatile unsigned char proceed = 1;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      ts_token_syntax_error(
          (const wchar_t *) L"Unexpected end of stream while parsing switch body",
          tsFile,
          token
      );
      break;
    }
    switch (tok[0]) {
      case L' ': {
        u_long movedBy = wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->character += movedBy;
        free((void *) tok);
        break;
      }
      case L'\n': {
        u_long movedBy = wcslen(tok);
        free((void *) tok);
        tsParseData->position += movedBy;
        tsParseData->character = 0;
        tsParseData->line += 1;
        break;
      }
      case L'}': {
        proceed = 0;
        u_long movedBy = wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->character += movedBy;
        free((void *) tok);
        break;
      }
      default: {
        tsParseData->token = tok;
        child = TS_parse_ts_token(tsFile, tsParseData);

        u_long movedBy = wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->character += movedBy;

        switch (child->tokenType) {
          case TS_CASE:
          case TS_DEFAULT: {
            break;
          }
          default: {
            ts_token_syntax_error(
                (const wchar_t *) L"Unexpected token while parsing switch body",
                tsFile,
                token
            );
            TS_free_tsToken(child);
            child = NULL;
            break;
          }
        }

        free((void *) tok);
        proceed = 0;
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
)
{
  TS_TOKEN_BEGIN("switch");

  TSParserToken *token = TS_build_parser_token(TS_SWITCH, tsParseData);
  TSParserToken *child = NULL;

  volatile long brackets_count = 0;

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
      }
      else if (brackets_count < 0) {
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
    TS_push_child(token, child);
  }

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("switch");

  return token;
}

void
TS_free_switch(
    const TSParserToken *token
)
{
  TS_free_children(token);
  free((void *) token);
}
