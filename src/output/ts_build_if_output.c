#include <cts/file.h>
#include <cts/output.h>

void TS_build_if_output(TSFile *tsFile) {
  TSParserToken *token = tsFile->output.currentToken;
  if (token->childrenSize != 2 && token->childrenSize != 3) {
    TS_highlight_error();
    fprintf(stderr, "if children size is invalid: %u\n", token->childrenSize);
    TS_clear_highlight();
    return;
  }

  TSParserToken *condition = token->children[0];
  TSParserToken *scope = token->children[1];

  TS_build_indent_output(tsFile);
  if (tsFile->settings.highlight) {
    TS_highlight_code(TS_COLOR_YELLOW);
  }
  TS_write(tsFile, (const wchar_t *) L"if");
  if (tsFile->settings.highlight) {
    TS_clear_highlight();
  }

  TS_write(tsFile, (const wchar_t *) L" (");
  tsFile->output.currentToken = condition;
  TS_build_condition_output(tsFile);

  TS_write(tsFile, (const wchar_t *) L") ");

  tsFile->output.currentToken = scope;
  TS_build_scope_output(tsFile);

  if (token->childrenSize == 3) {
    TSParserToken *elseToken = token->children[2];

    TS_build_indent_output(tsFile);

    tsFile->output.currentToken = elseToken;
    TS_build_else_output(tsFile);
  }
}