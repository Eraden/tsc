#include <cts/parser.h>

TSParserToken *
TS_parse_in(
    TSFile *_,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("in");
  TSParserToken *token = TS_build_parser_token(TS_IN, tsParseData);
  TS_TOKEN_END("in");
  return token;
}

void TS_free_in(
    const TSParserToken *token
) {
  TS_free_unknown(token);
}