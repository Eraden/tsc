#include <cts/parser.h>

TSParserToken *
TS_parse_of(
    TSFile *_,
    TSParseData *tsParseData
) {
  TS_MOVE_BY(tsParseData, tsParseData->token);
  TS_TOKEN_BEGIN("of");
  TSParserToken *token = TS_build_parser_token(TS_OF, tsParseData);

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("of");
  return token;
}

void
TS_free_of(
    const TSParserToken *token
) {
  TS_free_unknown(token);
}