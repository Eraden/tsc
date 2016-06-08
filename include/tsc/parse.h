#pragma once

#include <tsc/types.h>

void parseTSTokens(TSParseContext *context);
void parseTSToken(TSParseContext *context);
void parseTSExport(TSParseContext *context);
void parseTSDecorator(TSParseContext *context);
void parseTSClass(TSParseContext *context);
void parseTSMethod(TSParseContext *context);
void parseTSFunction(TSParseContext *context);
void getTSToken(TSParseContext *context);

TSParseContext *parseFile(const char *file);
// String
static char *appendChar(const char *content, char c);
static char *cloneString(const char *content);
