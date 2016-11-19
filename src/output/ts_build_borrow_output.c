#include <cts/file.h>
#include <cts/output.h>

void TS_build_borrow_output(TSFile *tsFile) {
  TSParserToken *borrow = tsFile->output.currentToken;

  if (borrow->childrenSize == 1) {
    tsFile->output.currentToken = borrow->children[0];
    TS_build_token_output(tsFile);
  }
}

