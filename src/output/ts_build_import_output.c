#include <cts/file.h>
#include <cts/output.h>

void TS_build_imported_tokens_output(TSFile *tsFile) {
}

void TS_build_import_from_output(TSFile *tsFile) {
}

void TS_build_import_output(TSFile *tsFile) {
  TSParserToken *import = tsFile->output.currentToken;
  TS_build_indent_output(tsFile);

  TS_write(tsFile, (const wchar_t *) L"\n");
}