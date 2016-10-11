#include <cts/register.h>

TSParserToken *
TS_create_borrow(
    TSParserToken *child,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_BORROW, tsParseData)
    TS_push_child(token, child);
    child->usageCount += 1;
  TS_TOKEN_END(TS_BORROW)
}

void
TS_free_borrow(
    const TSParserToken *token
) {
  if (token->childrenSize) free(token->children);
  free((void *) token);
}