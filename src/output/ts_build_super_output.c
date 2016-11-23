#include <cts/file.h>
#include <cts/output.h>

static TSParserToken *TS_find_current_class(TSParserToken *token) {
  TSParserToken *current = token->parent;
  TSParserToken *found = NULL;
  while (current) {
    if (current->tokenType == TS_CLASS) {
      found = current;
      current = NULL;
    } else {
      current = current->parent;
    }
  }
  return found;
}

static TSParserToken *TS_find_method(TSParserToken *token) {
  TSParserToken *current = token->parent;
  TSParserToken *found = NULL;
  while (current) {
    if (current->tokenType == TS_CLASS_METHOD) {
      found = current;
      current = NULL;
    } else {
      current = current->parent;
    }
  }
  return found;
}

static TSParserToken *TS_find_parent_class(TSParserToken *token) {
  TSParserToken *found = NULL;
  TS_EACH_CHILD(token)
    if (current->tokenType == TS_EXTENDS) {
      found = current->children[0]->children[0]; // extends -> borrow -> class
      break;
    }
  TS_END_EACH
  return found;
}

void TS_build_super_output(TSFile *tsFile) {
  TSParserToken *token = tsFile->output.currentToken;
  TSParserToken *method = TS_find_method(token);
  if (!method) {
    TS_highlighted_error("Failed to find method for `super`");
    return;
  }
  TSParserToken *currentClass = TS_find_current_class(method);
  if (!currentClass) {
    TS_highlighted_error("Failed to find class for `super`");
    return;
  }
  TSParserToken *parentClass = TS_find_parent_class(currentClass);
  if (!parentClass) {
    TS_highlighted_error("Failed to find super-class for `super`");
    return;
  }
  TS_build_indent_output(tsFile);
  TS_write(tsFile, parentClass->name);
  if (token->name) {
    TS_write(tsFile, (const wchar_t *) L".prototype.");
    TS_write(tsFile, token->name);
  }
  TS_write(tsFile, (const wchar_t *) L".call");
  TSParserToken *callArgs = token->children[0];
  tsFile->output.currentToken = callArgs;
  TS_build_call_arguments_output(tsFile);
}