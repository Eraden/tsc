#pragma once

#include <tsc/parser.h>
#include <tsc/sys.h>

#define TS_SPREAD_PREFIX_SIZE 3

//typedef enum TSOutputType {
//  TS_OUTPUT_TYPE_STDOUT = 0x0,
//  TS_OUTPUT_TYPE_FILE = 0x1,
//  TS_OUTPUT_TYPE_STRING = 0x2,
//};

typedef struct sTSOutputSettings {
  u_long indent;
} TSOutputSettings;

const char *TS_string_from_function(TSParserToken tsParserToken, TSOutputSettings outputSettings);
const char *TS_string_for_if(TSParserToken tsParserToken, TSOutputSettings outputSettings);
const char *TS_string_from_var(TSParserToken tsParserToken, TSOutputSettings outputSettings);
const char *TS_string_for_token(TSParserToken tsParserToken, TSOutputSettings outputSettings);

const char *TS_generate_string_from_file(TSFile *tsFile);
void TS_print_file(TSFile *tsFile);