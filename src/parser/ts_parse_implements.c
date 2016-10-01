#include <cts/parser.h>

TSParserToken *
TS_parse_implements(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_IMPLEMENTS, tsParseData)
  TS_TOKEN_END(TS_IMPLEMENTS);
}

void
TS_free_implements(
    const TSParserToken *token
) {
  free((void *) token);
}
