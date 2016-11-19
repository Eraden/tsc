#include <cts/file.h>
#include <cts/output.h>

void
TS_build_of_output(TSFile *tsFile) {
  TS_write(tsFile, (const wchar_t *) L"of");
}
