#pragma once

#include <tsc/parser.h>

void TS_parse_local_variable_body(TSParseData *tsParseData, u_long *movedBy, TSParserToken *token, TSFile *tsFile);
void TS_parse_local_variable_value(TSParseData *tsParseData, TSLocalVariableData *varData, u_long *movedBy);