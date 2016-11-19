#include <cts/file.h>
#include <cts/output.h>

void
TS_build_return_output(TSFile *tsFile) {
  TSParserToken *token = tsFile->output.currentToken;

  if (token->childrenSize == 1) {
    TS_build_indent_output(tsFile);

    TS_write(tsFile, (const wchar_t *) L"return ");

    tsFile->output.currentToken = token->children[0];
    TS_build_token_output(tsFile);
  }
}
