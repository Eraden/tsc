#pragma once

#include <tsc/types.h>
#include <tsc/str.h>

void parseTSTokens(TSParseContext *context);
void parseTSToken(TSParseContext *context);
void parseTSExport(TSParseContext *context);
void parseTSDecorator(TSParseContext *context);
void parseTSClass(TSParseContext *context);
void parseTSMethod(TSParseContext *context);
void parseTSFunction(TSParseContext *context);
void getTSToken(TSParseContext *context);

TSParseContext *parseFile(const char *file);

