#include <cts/register.h>

TSParserToken *
TS_create_borrow(
    TSParserToken *child,
    TSFile *tsFile
) {
  tsFile->parse.token = (const wchar_t *) L"";
  TS_TOKEN_BEGIN(TS_BORROW, tsFile)
    TS_push_child(token, child);
    child->usageCount += 1;
  TS_TOKEN_END(TS_BORROW)
}

void
TS_free_borrow(
    const TSParserToken *token
) {
  TS_free_children_from(token, 1);
  free((void *) token);
}