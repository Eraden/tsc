#include <cts/parser.h>

TSParserToken *
TS_parse_json(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("json");
  TSParserToken *token = TS_build_parser_token(TS_JSON, tsParseData);

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("json");
  return token;
}

void
TS_free_json(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
