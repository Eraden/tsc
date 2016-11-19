#include <cts/output.h>

void TS_build_condition_output(TSFile *tsFile) {
  TSParserToken *token = tsFile->output.currentToken;
  TSParserToken *entry = NULL;
  TSParserToken **children = token->children;
  unsigned int len = token->childrenSize;
  while (len) {
    entry = children[0];
    tsFile->output.currentToken = entry;
    TS_build_token_output(tsFile);
    children += 1;
    len -= 1;
    if (len != 0) TS_write(tsFile, (const wchar_t *) L", ");
  }
}