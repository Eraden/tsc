#include <cts/output.h>

// PRINT

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-if")))
ts_print_for_if_body(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
}

void
TS_print_for_if(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
}

// STRING

static const wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-if")))
ts_string_for_if_body(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    const TSOutputSettings outputSettings
) {
  return NULL;
}

const wchar_t *
TS_string_for_if(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  return NULL;
}