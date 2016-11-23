#include <cts/file.h>
#include <cts/output.h>

void TS_build_call_arguments_output(TSFile *tsFile) {
  TSParserToken *callArguments = tsFile->output.currentToken;
  TS_write(tsFile, (const wchar_t *) L"(");

  TS_EACH_CHILD(callArguments)
    TS_write(tsFile, current->name);
    if (childIndex) {
      TS_write(tsFile, (const wchar_t *) L", ");
    }
  TS_END_EACH

  TS_write(tsFile, (const wchar_t *) L")");
}