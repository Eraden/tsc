#include <tsc/parser.h>

const TSParserToken
TS_parse_extends(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("extends");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_EXTENDS;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  const wchar_t *tok;
  unsigned char proceed = 1;
  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Missing parent class name", tsFile, &token);
    }
    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
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
        if (!TS_name_is_valid(tok)) {
          free((void *) tok);
          ts_token_syntax_error((wchar_t *) L"Invalid parent class name", tsFile, &token);
        }
        token.data = (void *) TS_clone_string(tok);
        proceed = 0;
        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("extends");
  return token;
}

void TS_free_extends(const TSParserToken token) {
  // TODO
}
