#include <tsc/parser.h>

/**
 * TODO implement
 */
TSParserToken *
TS_parse_default(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("default");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_DEFAULT, tsParseData);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("default");
  return token;
}

void
TS_free_default(
    const TSParserToken *token
) {
  free((void *) token);
}
