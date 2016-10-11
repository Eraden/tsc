#include <cts/output.h>

// PRINT

void TS_print_indent(FILE *stream, const u_long indent) {
  for (u_long indentIndex = 0; indentIndex < indent; indentIndex++) {
    fprintf(stream, "%s", "  ");
    fflush(stream);
  }
}

void TS_push_indent_string(wchar_t *string, const u_long indent) {
  for (u_long indentIndex = 0; indentIndex < indent; indentIndex++)
    wcscat(string, (const wchar_t *) L"  ");
}

void
TS_print_for_token(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
}

// STRING

const wchar_t *
TS_string_for_token(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  return NULL;
}

void
TS_print_stream(
    TSFile *tsFile,
    FILE *stream
) {
}

const wchar_t *
__attribute__((__unused__))
TS_generate_string_from_file(
    TSFile *tsFile
) {
  return NULL;
}