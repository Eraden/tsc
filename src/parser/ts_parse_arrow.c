#include <cts/parser.h>

TSParserToken *
TS_parse_arrow(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_ARROW, tsFile)

  TS_TOKEN_END(TS_ARROW)
}

void
TS_free_arrow(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}