#include <cts/file.h>
#include <cts/output.h>

void TS_build_else_output(TSFile *tsFile) {
  TSParserToken *elseToken = tsFile->output.currentToken;
  if (elseToken->childrenSize != 1) {
    TS_highlight_error();
    fprintf(stderr, "else children size is invalid: %u\n", elseToken->childrenSize);
    TS_clear_highlight();
    return;
  }

  if (tsFile->settings.highlight) {
    TS_highlight_code(TS_COLOR_YELLOW);
  }
  TS_write(tsFile, (const wchar_t *) L"else\n");
  if (tsFile->settings.highlight) {
    TS_clear_highlight();
  }

  TS_build_indent_output(tsFile);

  TSParserToken *child = elseToken->children[0];
  tsFile->output.currentToken = child;
  TS_build_token_output(tsFile);
}