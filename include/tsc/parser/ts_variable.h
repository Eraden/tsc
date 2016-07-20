#pragma once

#include <tsc/parser.h>

void
__attribute__((nothrow))
    TS_parse_local_variable_body(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token,
    u_long *movedBy
);
