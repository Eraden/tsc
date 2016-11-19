#include <cts/file.h>
#include <cts/output.h>

void
TS_build_scope_output(TSFile *tsFile) {
  TSParserToken *scope = tsFile->output.currentToken;

  TS_write(tsFile, (const wchar_t *) L"{\n");
  tsFile->output.indent += 1;

  TSParserToken **children = scope->children;
  TSParserToken *child = NULL;
  unsigned int len = scope->childrenSize;

  while (len) {
    len -= 1;
    child = children[0];
    tsFile->output.currentToken = child;
    TS_build_token_output(tsFile);
    children += 1;
  }

  tsFile->output.indent -= 1;

  TS_write(tsFile, (const wchar_t *) L"\n");
  TS_build_indent_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L"}\n");
}
