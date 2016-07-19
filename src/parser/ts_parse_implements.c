#include <tsc/parser.h>

const TSParserToken
TS_parse_implements(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("implements");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_IMPLEMENTS;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("implements");
  return token;
}