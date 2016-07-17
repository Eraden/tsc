#include <tsc/parser.h>

const TSParserToken TS_parse_class(TSFile *__attribute__((__unused__)) tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "class");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_CLASS;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  log_to_file("-> end %s\n", "class");
  return token;
}