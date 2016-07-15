#include <tsc/parser.h>

/**
 * TODO implement
 */
const TSParserToken TS_parse_import(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "import");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_IMPORT;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  log_to_file("-> end %s\n", "import");
  return token;
}