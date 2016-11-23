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

  TS_build_indent_output(tsFile);

  if (tsFile->settings.highlight) {
    TS_highlight_code(TS_COLOR_YELLOW);
  }

  TS_write(tsFile, (const wchar_t *) L"else\n");


  if (tsFile->settings.highlight) {
    TS_clear_highlight();
  }

  TSParserToken *child = elseToken->children[0];
  tsFile->output.currentToken = child;
  switch (child->tokenType) {
    case TS_SCOPE: {
      TS_build_token_output(tsFile);
      break;
    }
    default: {
      tsFile->output.indent += 1;
      TS_build_token_output(tsFile);
      tsFile->output.indent -= 1;
    }
  }
}