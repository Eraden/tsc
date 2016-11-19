#include <cts/file.h>
#include <cts/output.h>

void TS_build_string_template_output(TSFile *tsFile) {
}

void
TS_build_string_output(TSFile *tsFile) {
  TSParserToken *string = tsFile->output.currentToken;
  TS_write(tsFile, string->content);
}
