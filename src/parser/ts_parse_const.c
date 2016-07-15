#include <tsc/parser/ts_variable.h>

const TSParserToken TS_parse_const(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "const");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_CONST;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  TS_parse_local_variable_body(tsParseData, &movedBy, &token, tsFile);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  log_to_file("-> end %s\n", "const");
  return token;
}