#include <cts/output.h>

// PRINT

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-else")))
ts_print_for_else_body(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
}

void
__attribute__(( section("output-else")))
TS_print_for_else(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
}

// STRING

static const wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-else")))
ts_string_for_else_body(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
  return NULL;
}

const wchar_t *
__attribute__(( section("output-else")))
TS_string_for_else(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  return NULL;
}