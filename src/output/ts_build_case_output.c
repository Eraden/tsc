#include <cts/output.h>
#include <cts/file.h>

void TS_build_case_output(TSFile *tsFile) {
  TSParserToken *caseToken = tsFile->output.currentToken;
  if (caseToken->childrenSize == 2) {
    TSParserToken *condition = caseToken->children[0];
    TSParserToken *scope = caseToken->children[1];

    TS_build_indent_output(tsFile);

    TS_write(tsFile, (const wchar_t *) L"case ");

    tsFile->output.currentToken = condition;
    TS_build_condition_output(tsFile);

    TS_write(tsFile, (const wchar_t *) L":\n");

    tsFile->output.currentToken = scope;
    TS_build_scope_output(tsFile);
  }
}