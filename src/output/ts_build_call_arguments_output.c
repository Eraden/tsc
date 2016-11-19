#include <cts/output.h>

void TS_build_call_arguments_output(TSFile *tsFile) {
  TS_write(tsFile, (const wchar_t *) L"(");

  TS_write(tsFile, (const wchar_t *) L")");
}