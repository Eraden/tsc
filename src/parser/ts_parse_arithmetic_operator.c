#include <cts/register.h>

TSParserToken *
TS_parse_operator(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_OPERATOR, tsFile)
    token->content = TS_clone_string(tsFile->parse.token);
  TS_TOKEN_END(TS_ARITMETIC_OPERATOR)
}

void
TS_free_operator(
    const TSParserToken *token
) {
  if (token->content)
    free(token->content);
  TS_free_children(token);
  free((void *) token);
}