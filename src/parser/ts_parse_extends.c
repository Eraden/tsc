#include <cts/parser.h>

TSParserToken *
TS_parse_extends(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("extends");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_EXTENDS, tsParseData);

  const wchar_t *tok;
  unsigned char proceed = 1;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error(
          (wchar_t *) L"Unexpected end of stream while parsing class parent name",
          tsFile,
          token
      );
      break;
    }

    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        tsParseData->character += movedBy;
        tsParseData->position += movedBy;
        movedBy = 0;
        free((void *) tok);
        break;
      }
      case L'\n': {
        movedBy += wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->character = 0;
        tsParseData->line += 1;
        movedBy = 0;
        free((void *) tok);
        break;
      }
      default: {
        movedBy += wcslen(tok);
        if (!TS_name_is_valid(tok)) {
          free((void *) tok);
          ts_token_syntax_error(
              (wchar_t *) L"Invalid parent class name",
              tsFile,
              token
          );
        } else {
          token->name = TS_clone_string(tok);
          tsParseData->character += movedBy;
          tsParseData->position += movedBy;
          movedBy = 0;
        }
        proceed = 0;
        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  tsParseData->parentTSToken = token->parent;

  TS_TOKEN_END("extends");
  return token;
}

void
TS_free_extends(
    const TSParserToken *token
) {
  if (token->name) free(token->name);
  free((void *) token);
}
