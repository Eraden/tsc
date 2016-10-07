#include <cts/output.h>

// PRINT

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_print_for_function_head(
    TSFile *__attribute__((__unused__)) tsFile,
    const TSParserToken *token,
    const u_long indent,
    TSOutputSettings outputSettings
) {

}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_print_for_function_spread_arg(
    const u_long indent,
    const TSParserToken *token,
    TSOutputSettings outputSettings
) {
}

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_print_for_function_arg_default(
    const u_long indent,
    TSOutputSettings outputSettings
) {
}
#pragma clang diagnostic pop

static void
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_print_for_function_body(
    TSFile *tsFile,
    const TSParserToken *token,
    const u_long indent,
    const TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
}

void
__attribute__(( section("output-function")))
TS_print_from_function(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
}

// STRING

static u_long
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_output_measure_function_head(
    TSFile *__attribute__((__unused__)) tsFile,
    const u_long indent,
    const TSParserToken *token
) {
  return 0;
}

static wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_string_for_function_head(
    TSFile *__attribute__((__unused__)) tsFile,
    const TSParserToken *token,
    const u_long indent,
    const u_long size
) {
  return NULL;
}

static wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_string_for_function_spread_arg(
    const u_long indent,
    const TSParserToken *token
) {
  return NULL;
}

static wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_string_for_function_arg_default(
    const u_long indent
) {
  return NULL;
}

static wchar_t *
__attribute(( visibility("hidden")))
__attribute__(( section("output-function")))
TS_string_for_function_body(
    TSFile *tsFile,
    const TSParserToken *token,
    const u_long indent,
    wchar_t *string,
    TSOutputSettings outputSettings
) {
    return NULL;
}

const wchar_t *
__attribute__(( section("output-function")))
TS_string_from_function(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  return NULL;
}

