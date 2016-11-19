#include <cts/file.h>
#include <cts/output.h>

void TS_build_namespace_output(TSFile *tsFile) {
  TSParserToken *interface = tsFile->output.currentToken;
  if (interface->childrenSize == 0) return;
  unsigned int exportCount = 0;
  unsigned int size = interface->childrenSize;
  TSParserToken *current = NULL;
  TSParserToken **children = interface->children;

  while (size) {
    current = children[0];
    if (current->tokenType == TS_EXPORT) exportCount += 1;
    size -= 1;
    children += 1;
  }

  if (!exportCount) return;

  TS_build_indent_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L"var ");
  TS_write(tsFile, interface->name);
  TS_write(tsFile, (const wchar_t *) L" = {};");
}
