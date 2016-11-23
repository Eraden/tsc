#include <cts/file.h>
#include <cts/output.h>

static TSParserToken *
TS_find_constructor(TSParserToken *token) {
  if (token == NULL)
    return NULL;

  TSParserToken *parent = NULL;
  TSParserToken *current = NULL;
  unsigned int size = token->childrenSize;
  TSParserToken **children = token->children;

  while (size) {
    current = children[0];
    switch (current->tokenType) {
      case TS_EXTENDS: {
        parent = current->children[0]->children[0]; // extends -> borrow -> parent
        break;
      }
      case TS_CLASS_METHOD: {
        if (current->name == NULL)
          break;
        if (wcscmp(current->name, (const wchar_t *) L"constructor") == 0) {
          return current;
        }
        break;
      }
      default: {
        break;
      }
    }
    size -= 1;
    children += 1;
  }
  return TS_find_constructor(parent);
}

void TS_build_class_output(TSFile *tsFile) {
  TSParserToken *token = tsFile->output.currentToken;
  TSParserToken *constructor = TS_find_constructor(token);

  TS_build_indent_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L"function ");
  TS_write(tsFile, token->name);
  if (constructor) {
    tsFile->output.currentToken = constructor->children[TS_CLASS_METHOD_CALL_ARGS_INDEX]; // call arguments
    TS_build_call_arguments_output(tsFile);
    TS_write(tsFile, (const wchar_t *) L" ");
    tsFile->output.currentToken = constructor->children[TS_CLASS_METHOD_BODY_INDEX]; // scope / body
    TS_build_scope_output(tsFile);
  } else {
    TS_write(tsFile, (const wchar_t *) L"() {}\n");
  }
}

void TS_build_class_field_output(TSFile *tsFile) {

}

void TS_build_class_method_output(TSFile *tsFile) {

}
