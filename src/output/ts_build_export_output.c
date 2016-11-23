#include <cts/file.h>
#include <cts/output.h>

void TS_build_export_output(TSFile *tsFile) {
  TSParserToken *token = tsFile->output.currentToken;
  if (token->childrenSize != 1) {
    return;
  }

  TSParserToken *exported = token->children[0];

  switch (exported->tokenType) {
    case TS_FUNCTION:
    case TS_CLASS:
    case TS_VAR:
    case TS_LET:
    case TS_CONST: {
      tsFile->output.currentToken = exported;
      TS_build_token_output(tsFile);
      TS_build_indent_output(tsFile);

      TS_write(tsFile, (const wchar_t *) L"module.");
      TS_write(tsFile, exported->name);
      TS_write(tsFile, (const wchar_t *) L" = ");
      TS_write(tsFile, exported->name);
      TS_write(tsFile, (const wchar_t *) L";\n");
      break;
    }
    case TS_DEFAULT: {
      TSParserToken *exportedToken = exported->children[0];
      tsFile->output.currentToken = exportedToken;
      TS_build_token_output(tsFile);
      TS_build_indent_output(tsFile);

      TS_write(tsFile, (const wchar_t *) L"Object.defineProperty(module, \"");
      TS_write(tsFile, (const wchar_t *) L"@@(default)");
      TS_write(tsFile, (const wchar_t *) L"\", { get: function () { return ");
      TS_write(tsFile, exportedToken->name);
      TS_write(tsFile, (const wchar_t *) L"; } });\n");
      break;
    }
    default: {
      break;
    }
  }
}