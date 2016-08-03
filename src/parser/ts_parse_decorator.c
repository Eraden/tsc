#include <tsc/parser.h>

/**
 * TODO implement
 */
const TSParserToken
__attribute__((section("parse-decorator")))
TS_parse_decorator(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("component");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_DECORATOR;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  TS_TOKEN_END("component");
  return token;
}

void TS_free_decorator(const TSParserToken token) {
  TS_free_children(token);
  // TODO
}
