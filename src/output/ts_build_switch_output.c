#include <cts/file.h>
#include <cts/output.h>

void
TS_build_switch_body_output(TSFile *tsFile) {
  TSParserToken *body = tsFile->output.currentToken;

  TS_write(tsFile, (const wchar_t *) L"{\n");

  tsFile->output.indent += 1;

  TSParserToken **children = body->children;
  TSParserToken *child = NULL;
  unsigned int len = body->childrenSize;

  while (len) {
    child = children[0];

    tsFile->output.currentToken = child;
    TS_build_token_output(tsFile);

    len -= 1;
    children += 1;
  }

  tsFile->output.indent -= 1;

  TS_write(tsFile, (const wchar_t *) L"}\n");
}


void
TS_build_switch_output(TSFile *tsFile) {
  TSParserToken *token = NULL, *condition = NULL, *body = NULL;
  token = tsFile->output.currentToken;
  if (token->childrenSize != 2) return;

  condition = token->children[0];
  if (condition->tokenType != TS_CONDITION) return;

  body = token->children[1];
  if (body->tokenType != TS_SWITCH_BODY) return;

  TS_build_indent_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L"switch (");

  tsFile->output.currentToken = condition;
  TS_build_condition_output(tsFile);

  TS_write(tsFile, (const wchar_t *) L") ");

  tsFile->output.currentToken = body;
  TS_build_switch_body_output(tsFile);
}
