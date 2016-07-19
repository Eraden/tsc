#include <tsc/parser.h>

/**
 * TODO implement
 */
const TSParserToken TS_parse_default(TSFile *__attribute__((__unused__)) tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "default");
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
  log_to_file("-> end %s\n", "default");
  return token;
}