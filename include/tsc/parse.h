#pragma once

#include <tsc/sys.h>
#include <tsc/types.h>
#include <tsc/str.h>

void parseTSTokens(TSParseContext *context);
void parseTSToken(TSParseContext *context);
void parseTSExport(TSParseContext *context);
void parseTSDecorator(TSParseContext *context);
void parseTSClass(TSParseContext *context);
void parseTSFunction(TSParseContext *context);
void getTSToken(TSParseContext *context);

TSParseContext *parseFile(char *file);

