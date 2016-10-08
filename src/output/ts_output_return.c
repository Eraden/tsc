#include <cts/output.h>

// PRINT

void
__attribute(( visibility("hidden") ))
__attribute__(( section("output-return") ))
ts_print_for_return_body(
    TSFile *tsFile,
    const TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
}

void
ts_print_for_return(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
}

// STRING

static const wchar_t *
__attribute(( visibility("hidden") ))
__attribute__(( section("output-return") ))
ts_string_for_return_body(
    TSFile *tsFile,
    const TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  return NULL;
}

const wchar_t *
ts_string_for_return(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  return NULL;
}