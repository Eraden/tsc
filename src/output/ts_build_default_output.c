#include <cts/output.h>

void TS_build_default_output(TSFile *tsFile) {
  TSParserToken *token = tsFile->output.currentToken;
  if (token && token->childrenSize == 1) {
    TSParserToken *scope = token->children[0];

    TS_build_indent_output(tsFile);

    TS_write(tsFile, (const wchar_t *) L"default:\n");
    tsFile->output.currentToken = scope;
    TS_build_scope_output(tsFile);
  }
}