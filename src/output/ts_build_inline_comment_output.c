#include <cts/file.h>
#include <cts/output.h>

void TS_build_inline_comment_output(TSFile *tsFile) {
  if (!tsFile->settings.preserveComments) return;
  TSParserToken *comment = tsFile->output.currentToken;
  TS_build_indent_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L"// ");
  TS_write(tsFile, comment->content);
  TS_write(tsFile, (const wchar_t *) L"\n");
}
