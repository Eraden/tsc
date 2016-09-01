#include <tsc/parser.h>

/**
 * TODO implement
 */
TSParserToken *
__attribute__((section("parse-decorator")))
TS_parse_decorator(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("component");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_DECORATOR, tsParseData);

  // parse

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("component");
  return token;
}

void
TS_free_decorator(
    TSParserToken *token
) {
  TS_free_children(token);
  free(token);
}
