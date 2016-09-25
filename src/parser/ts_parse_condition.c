#include <cts/parser.h>

TSParserToken *
TS_parse_condition(
    TSFile *tsFile,
    TSParseData *tsParseData
)
{
  TS_TOKEN_BEGIN("condition");

  TSParserToken *token = TS_build_parser_token(TS_CONDITION, tsParseData);

  const wchar_t *tok;
  unsigned char proceed = 1;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of conditions", tsFile, token);
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
        tsParseData->line += 1;
        tsParseData->character = 0;
        break;
      }
      default: {
        u_long movedBy = wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->character += movedBy;

        token->name = (void *) TS_clone_string(tok);

        proceed = 0;

        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("condition");

  return token;
}

void TS_free_condition(const TSParserToken *token)
{
  TS_free_unknown(token);
}

