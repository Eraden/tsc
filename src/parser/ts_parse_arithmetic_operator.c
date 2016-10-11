#include <cts/register.h>

TSParserToken *
TS_parse_operator(
    TSFile *__attribute__((__unused__))tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_OPERATOR, tsParseData)
    token->content = TS_clone_string(tsParseData->token);
  TS_TOKEN_END(TS_ARITMETIC_OPERATOR)
}

void
TS_free_operator(
    const TSParserToken *token
) {
  if (token->content)
    free(token->content);
  free((void *) token);
}