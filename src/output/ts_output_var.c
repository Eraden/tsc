#include <cts/output.h>

// PRINT

void
__attribute__(( section("output-var")))
TS_print_from_var(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
}

// STRING

const wchar_t *
__attribute__(( section("output-var")))
TS_string_from_var(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  return NULL;
}