#pragma once

#include <cts/file.h>
#include <cts/parser.h>

#define TS_SPREAD_PREFIX_SIZE 3

void TS_write(TSFile *tsFile, const wchar_t *buffer);

void TS_clear_output(TSFile *tsFile);

void TS_set_output_stream(TSFile *tsFile, FILE *stream);

void TS_set_output_string(TSFile *tsFile, wchar_t *str);

void TS_set_output_file(TSFile *tsFile, char *fileName);

extern const wchar_t *
__attribute__((__malloc__))
TS_output_header(void);

const wchar_t *
__attribute__((__malloc__))
TS_output_footer(void);
