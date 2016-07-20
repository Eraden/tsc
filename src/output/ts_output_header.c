#include <tsc/output.h>

static const char *
__attribute__(( visibility("hidden")))
__attribute__(( section("output-header")))
TS_output_header_symbol_polyfill() {
  return "if (!('Symbol' in window)) {\n"
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

const char *
__attribute__((__malloc__))
TS_output_header() {
  char *string = NULL;
  const char *symbolPolyfill = TS_output_header_symbol_polyfill();
  u_long size = 0 +
                strlen("(function (modules, window) {\n\n") +
                strlen(symbolPolyfill) +
                strlen("\n") +
                1;
  string = (char *) calloc(sizeof(char), size);
  strcat(string, "(function (modules, window) {\n\n");
  strcat(string, symbolPolyfill); // not allocated so ok to leave that way
  strcat(string, "\n");
  return string;
}