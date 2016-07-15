#include <tsc/parser.h>

const TSParserToken TS_parse_return(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "return");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_RETURN;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  log_to_file("-> end %s\n", "return");
  return token;
}