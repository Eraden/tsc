#include <cts/parser.h>

TSParserToken *
TS_parse_in(
    TSFile *_,
    TSParseData *tsParseData
) {
  TS_MOVE_BY(tsParseData, tsParseData->token);
  TS_TOKEN_BEGIN("in");
  TSParserToken *token = TS_build_parser_token(TS_IN, tsParseData);

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("in");
  return token;
}

void TS_free_in(
    const TSParserToken *token
) {
  TS_free_unknown(token);
}