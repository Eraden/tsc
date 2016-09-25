#include <cts/parser.h>

TSParserToken *
TS_parse_arrow(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("arrow");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_ARROW, tsParseData);
  token->visibility = TS_VISIBILITY_SCOPE;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("arrow");
  return token;
}

void
TS_free_arrow(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}