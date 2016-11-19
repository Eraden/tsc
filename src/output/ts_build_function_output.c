#include <cts/file.h>
#include <cts/output.h>

void TS_build_function_return_type_output(TSFile *tsFile) {
  if (!tsFile->settings.preserveComments) return;
  TS_write(tsFile, (const wchar_t *) L"/* ");
  TS_build_borrow_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L" */");
}

void TS_build_function_output(TSFile *tsFile) {
  TSParserToken *function = tsFile->output.currentToken;
  if (function->childrenSize != 3) {
    fprintf(stderr, "function children size: %u\n", function->childrenSize);
    return;
  }
  TSParserToken *callArgs = function->children[0];
  TSParserToken *returnType = function->children[1];
  TSParserToken *scope = function->children[2];

  TS_build_indent_output(tsFile);

  TS_write(tsFile, (const wchar_t *) L"function ");
  TS_write(tsFile, function->name);

  tsFile->output.currentToken = callArgs;
  TS_build_call_arguments_output(tsFile);

  tsFile->output.currentToken = returnType;
  TS_build_function_return_type_output(tsFile);

  TS_write(tsFile, (const wchar_t *) L" ");

  tsFile->output.currentToken = scope;
  TS_build_scope_output(tsFile);
}