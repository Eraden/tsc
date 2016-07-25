#pragma once

#include <tsc/parser.h>
#include <tsc/sys.h>

#define TS_SPREAD_PREFIX_SIZE 3

typedef struct sTSOutputSettings {
  u_long indent;
  FILE *stream;
} TSOutputSettings;

const char *
    __attribute__((__malloc__))
    TS_output_header();

const char *
    __attribute__((__malloc__))
    TS_output_footer();

void
    TS_print_from_function(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

const char *
    TS_string_from_function(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

void
    TS_print_for_if(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

const char *
    TS_string_for_if(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

void
    TS_print_from_var(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

const char *
    TS_string_from_var(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

void
    TS_print_for_token(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

const char *
    TS_string_for_token(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

void
    ts_print_for_return(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

const char *
    ts_string_for_return(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

void
    TS_print_for_else(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

const char *
    TS_string_for_else(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

void
    TS_print_for_class(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

const char *
    TS_string_for_class(
    const TSFile *tsFile,
    const TSParserToken tsParserToken,
    TSOutputSettings outputSettings
);

const char *
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