#include <cts/output.h>

// PRINT

static void
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_print_for_class_method(
    TSFile *tsFile,
    const TSOutputSettings __attribute__((__weak__)) outputSettings,
    const TSParserToken *child
) {
}

static void
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_print_for_class_field(
    TSFile *__attribute__(( __unused__ )) tsFile,
    const TSOutputSettings __attribute__((__weak__)) outputSettings,
    const TSParserToken *child
) {
}

static void
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_print_for_class_prototype(
    TSFile *__attribute__(( __unused__ )) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
}

static void
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_print_for_class_constructor(
    TSFile *__attribute__(( __unused__ )) tsFile,
    const TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
}

void
__attribute__(( section("output-class")))
TS_print_for_class(
    TSFile *__attribute__(( __unused__ )) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
}

// STRING

static const wchar_t *
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_string_for_class_method(
    const TSOutputSettings __attribute__((__weak__)) outputSettings,
    const TSParserToken *child,
    u_long *methodSize
) {
  return NULL;
}

static const wchar_t *
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_string_for_class_field(
    const TSOutputSettings __attribute__((__weak__)) outputSettings,
    const TSParserToken *child,
    u_long *fieldSize
) {
  return NULL;
}

static const wchar_t *
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_string_for_class_prototype(
    TSFile *__attribute__(( __unused__ )) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings __attribute__((__weak__)) outputSettings
) {
  return NULL;
}

static const wchar_t *
__attribute__(( visibility("hidden")))
__attribute__(( section("output-class")))
TS_string_for_class_constructor(
    TSFile *__attribute__(( __unused__ )) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  return NULL;
}

const wchar_t *
__attribute__(( section("output-class")))
TS_string_for_class(
    TSFile *__attribute__(( __unused__ )) tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  return NULL;
}