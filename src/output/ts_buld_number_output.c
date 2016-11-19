#include <cts/file.h>
#include <cts/output.h>

void TS_build_number_output(TSFile *tsFile) {
  TS_write(tsFile, tsFile->output.currentToken->content);
}
