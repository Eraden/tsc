#include <tsc/parser.h>

TSParserToken *
TS_parse_case(
    TSFile *tsFile,
    TSParseData *tsParseData
)
{
  TS_TOKEN_BEGIN("case");
  TSParserToken *token = TS_build_parser_token(TS_CASE, tsParseData);

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("case");
  return token;
}

void
TS_free_case(
    const TSParserToken *token
)
{
  TS_free_children(token);
  free((void *) token);
}
