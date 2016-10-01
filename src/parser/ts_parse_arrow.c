#include <cts/parser.h>

TSParserToken *
TS_parse_arrow(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_ARROW, tsParseData)

  TS_TOKEN_END(TS_ARROW)
}

void
TS_free_arrow(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}