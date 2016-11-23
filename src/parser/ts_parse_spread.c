#include <cts/file.h>
#include <cts/output.h>

TSParserToken *
TS_parse_spread(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_SPREAD, tsFile)
    wchar_t *name = (wchar_t *) TS_get_token(tsFile->input.stream);
    TS_put_back(tsFile->input.stream, name);
    tsFile->parse.token = name;
    TSParserToken *argument = TS_parse_argument(tsFile);
    token->name = TS_clone_string(argument->name);
    token->children = argument->children;
    token->childrenSize = argument->childrenSize;
    argument->childrenSize = 0;
    argument->children = NULL;
    TS_free_argument(argument);
    free(name);
  TS_TOKEN_END(TS_SPREAD)
}

void TS_free_spread(const TSParserToken *token) {
  TS_free_argument(token);
}