#include <cts/parser.h>

/**
 * @todo implement
 * @param tsFile current file
 * @param tsParseData parse context
 * @return token tree
 */
TSParserToken *
TS_parse_json(
    TSFile __attribute__((__unused__))*tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_JSON, tsParseData)
  TS_TOKEN_END(TS_JSON)
}

void
TS_free_json(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
