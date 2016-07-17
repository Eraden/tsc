#import <tsc/parser.h>

/**
 * TODO implement
 */
const TSParserToken TS_parse_export(TSFile *__attribute__((__unused__)) tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "export");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_EXPORT;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  log_to_file("-> end %s\n", "export");
  return token;
}