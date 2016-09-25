#include <cts/parser.h>

/**
 * TODO implement
 */
TSParserToken *
TS_parse_import(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("import");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_IMPORT, tsParseData);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  tsParseData->parentTSToken= token->parent;
  TS_TOKEN_END("import");
  return token;
}

void
TS_free_import(
    const TSParserToken *token
) {
  free((void *) token);
}
