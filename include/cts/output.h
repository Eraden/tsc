#pragma once

#include <cts/parser.h>
#include <cts/sys.h>

#define TS_SPREAD_PREFIX_SIZE 3

typedef struct sTSOutputSettings {
  u_long indent;
  FILE *stream;
} TSOutputSettings;

const wchar_t *
__attribute__((__malloc__))
TS_output_header(void);

const wchar_t *
__attribute__((__malloc__))
TS_output_footer(void);

void
TS_print_from_function(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
TS_string_from_function(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

void
TS_print_for_if(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
TS_string_for_if(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

void
TS_print_from_var(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
TS_string_from_var(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

void
TS_print_for_token(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
TS_string_for_token(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

void
ts_print_for_return(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
ts_string_for_return(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

void
TS_print_for_else(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
TS_string_for_else(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

void
TS_print_for_class(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
TS_string_for_class(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
__attribute__((__used__))
TS_generate_string_from_file(
    TSFile *tsFile
);

void
__attribute__((__used__))
TS_print_stream(
    TSFile *tsFile,
    FILE *stream
);

void TS_print_indent(FILE *stream, const u_long indent);

void TS_push_indent_string(wchar_t *string, const u_long indent);