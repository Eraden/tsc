#include <cts/parser.h>

TSParserToken *
TS_parse_in(
    TSFile __attribute__((__unused__))*tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_IN, tsParseData)
  TS_TOKEN_END(TS_IN)
}

void TS_free_in(
    const TSParserToken *token
) {
  TS_free_unknown(token);
}