#include <cts/output.h>

// STRING

static const wchar_t *
__attribute__(( section("output-header")))
TS_output_header_symbol_polyfill(void);

static const wchar_t *
__attribute__(( section("output-header")))
TS_output_header_symbol_polyfill(void) {
  return (const wchar_t *) L"if (!('Symbol' in window)) {\n"
        "  var symbolCounter = 0;\n"
        "  function HiddenSymbol(description) { this.description = description; this.index = symbolCounter++; }\n"
        "  HiddenSymbol.prototype.toString = function () { return \"@@Symbol(\" + (this.description || \"\") + this.index + \")\"; };\n"
        "  window.Symbol = function (desc) {\n"
        "    return new HiddenSymbol(desc);\n"
        "  }\n"
        "}\n"
        "if (!('toStringTag' in window.Symbol)) window.Symbol.toStringTag = Symbol('Symbol.toStringTag');\n"
        "if (!('iterator' in window.Symbol)) window.Symbol.iterator = Symbol('Symbol.iterator');\n";
}

extern const wchar_t *
__attribute__((__malloc__))
TS_output_header(void) {
  wchar_t *string = NULL;
  const wchar_t *symbolPolyfill = TS_output_header_symbol_polyfill();
  u_long size = TS_STRING_END +
                wcslen((wchar_t *) L"(function (modules, window) {\n\n") +
                wcslen(symbolPolyfill) +
                wcslen((wchar_t *) L"\n");

  string = (wchar_t *) calloc(sizeof(wchar_t), size);
  wcscat(string, (wchar_t *) L"(function (modules, window) {\n\n");
  wcscat(string, symbolPolyfill); // not allocated so ok to leave that way
  wcscat(string, (wchar_t *) L"\n");
  return string;
}