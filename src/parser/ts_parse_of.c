#include <cts/parser.h>

TSParserToken *
TS_parse_of(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_OF, tsFile)
  TS_TOKEN_END(TS_OF)
}

void
TS_free_of(
    const TSParserToken *token
) {
  TS_free_unknown(token);
}