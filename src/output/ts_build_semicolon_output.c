#include <cts/file.h>
#include <cts/output.h>

void TS_build_semicolon_output(TSFile *tsFile) {
  TSParserToken *semicolon = tsFile->output.currentToken;

  if (semicolon->parent) {
    switch (semicolon->parent->tokenType) {
      case TS_SCOPE: {
        TS_write(tsFile, (const wchar_t *) L";\n");
        TS_build_indent_output(tsFile);
        break;
      }
      default: {
        TS_write(tsFile, (const wchar_t *) L";");
        break;
      }
    }
  } else {
    TS_write(tsFile, (const wchar_t *) L";\n");
    TS_build_indent_output(tsFile);
  }
}