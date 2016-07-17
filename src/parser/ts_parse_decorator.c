#include <tsc/parser.h>

/**
 * TODO implement
 */
const TSParserToken TS_parse_decorator(TSFile *__attribute__((__unused__)) tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "component");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_COMPONENT;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  log_to_file("-> end %s\n", "component");
  return token;
}