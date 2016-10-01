#include <cts/parser.h>

/**
 * TODO implement
 */
TSParserToken *
TS_parse_import(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_IMPORT, tsParseData)
  TS_TOKEN_END(TS_IMPORT)
}

void
TS_free_import(
    const TSParserToken *token
) {
  free((void *) token);
}
