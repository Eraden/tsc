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

const char *TS_string_from_function(const TSFile *tsFile, const TSParserToken tsParserToken, TSOutputSettings outputSettings);
const char *TS_string_for_if(const TSFile *tsFile, const TSParserToken tsParserToken, TSOutputSettings outputSettings);
const char *TS_string_from_var(const TSFile *tsFile, const TSParserToken tsParserToken, TSOutputSettings outputSettings);
const char *TS_string_for_token(const TSFile *tsFile, const TSParserToken tsParserToken, TSOutputSettings outputSettings);
const char *ts_string_for_return(const TSFile *tsFile, const TSParserToken tsParserToken, TSOutputSettings outputSettings);
const char *TS_string_for_else(const TSFile *tsFile, const TSParserToken tsParserToken, TSOutputSettings outputSettings);

const char *TS_generate_string_from_file(const TSFile *tsFile);
void TS_print_stream(const TSFile *tsFile, FILE *stream);