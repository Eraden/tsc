#include <cts/file.h>
#include <cts/output.h>

void TS_build_in_output(TSFile *tsFile) {
  TS_write(tsFile, (const wchar_t *) L"in");
}