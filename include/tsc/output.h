#pragma once

#include <tsc/parser.h>
#include <tsc/sys.h>

#define TS_SPREAD_PREFIX_SIZE 3

typedef struct sTSOutputSettings {
  u_long indent;
  FILE *stream;
} TSOutputSettings;

const wchar_t *
__attribute__((__malloc__))
TS_output_header();

const wchar_t *
__attribute__((__malloc__))
TS_output_footer();

void
TS_print_from_function(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
TS_string_from_function(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

void
TS_print_for_if(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
TS_string_for_if(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

void
TS_print_from_var(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
TS_string_from_var(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

void
TS_print_for_token(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
TS_string_for_token(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

void
ts_print_for_return(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
ts_string_for_return(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

void
TS_print_for_else(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
TS_string_for_else(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

void
TS_print_for_class(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
TS_string_for_class(
    const TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
);

const wchar_t *
__attribute__((__used__))
TS_generate_string_from_file(
    const TSFile *tsFile
);

void
__attribute__((__used__))
TS_print_stream(
    const TSFile *tsFile,
    FILE *stream
);

void TS_print_indent(FILE *stream, const u_long indent);

void TS_push_indent_string(wchar_t *string, const u_long indent);