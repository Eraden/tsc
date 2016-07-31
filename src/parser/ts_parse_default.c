#include <tsc/parser.h>

/**
 * TODO implement
 */
const TSParserToken TS_parse_default(TSFile *__attribute__((__unused__)) tsFile, TSParseData *tsParseData) {
  TS_TOKEN_BEGIN("default");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_DEFAULT;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  TS_TOKEN_END("default");
  return token;
}

void TS_free_default(const TSParserToken token) {
  // TODO
}
