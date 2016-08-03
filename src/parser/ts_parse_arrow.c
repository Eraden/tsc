#include <tsc/parser.h>

const
TSParserToken TS_parse_arrow(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("arrow");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_ARROW;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("arrow");
  return token;
}

void TS_free_arrow(const TSParserToken token) {
  TS_free_children(token);
  // TODO
}