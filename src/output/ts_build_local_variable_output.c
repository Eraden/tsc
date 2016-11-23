#include <cts/file.h>
#include <cts/output.h>

static void
TS_build_local_variable_body_output(TSFile *tsFile) {
  TSParserToken *variable = tsFile->output.currentToken;
  if (variable->childrenSize != 2) {
    return;
  }
  TS_build_indent_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L"var ");
  TS_write(tsFile, variable->name);
  TSParserToken *value = variable->children[TS_VARIABLE_VALUE_INDEX];
  TS_write(tsFile, (const wchar_t *) L" = ");
  tsFile->output.currentToken = value;
  TS_build_token_output(tsFile);
}

void
TS_build_var_output(TSFile *tsFile) {
  TS_build_local_variable_body_output(tsFile);
}

void
TS_build_let_output(TSFile *tsFile) {
  TS_build_local_variable_body_output(tsFile);
}

void
TS_build_const_output(TSFile *tsFile) {
  TS_build_local_variable_body_output(tsFile);
}
