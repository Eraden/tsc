#include <tsc/parser.h>

TSParserToken *
TS_parse_implements(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("implements");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_IMPLEMENTS, tsParseData);
  token->data = NULL;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  tsParseData->parentTSToken = token->parent;

  TS_TOKEN_END("implements");
  return token;
}

void
TS_free_implements(
    const TSParserToken *token
) {
  free((void *) token);
}
