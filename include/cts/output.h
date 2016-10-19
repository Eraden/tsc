#pragma once

#include <cts/parser.h>
#include <cts/sys.h>

#define TS_SPREAD_PREFIX_SIZE 3

typedef struct sTSOutputSettings {
  u_long indent;
  FILE *stream;
} TSOutputSettings;

extern const wchar_t *
__attribute__((__malloc__))
TS_output_header(void);

const wchar_t *
__attribute__((__malloc__))
TS_output_footer(void);

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