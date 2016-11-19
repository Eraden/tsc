#include <cts/file.h>
#include <cts/output.h>

void
TS_build_new_output(TSFile *tsFile) {
  TSParserToken *token = tsFile->output.currentToken;

  if (token->childrenSize != 1) return;
  TSParserToken *classBorrow = token->children[0];
  if (classBorrow->tokenType != TS_BORROW) return;

  TS_write(tsFile, (const wchar_t *) L"new ");
  tsFile->output.currentToken = classBorrow;
  TS_build_borrow_output(tsFile);
}

