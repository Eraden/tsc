#include <cts/output.h>

void TS_build_break_output(TSFile *tsFile) {
  TS_build_indent_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L"break");
}