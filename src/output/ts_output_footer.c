#include <tsc/output.h>

const wchar_t *
__attribute__((__malloc__))
TS_output_footer(void) {
  wchar_t *string = NULL;
  u_long size = TS_STRING_END +
                wcslen((wchar_t *) L"\n}(modules, window));\n");
  string = (wchar_t *) calloc(sizeof(wchar_t), size);
  wcscat(string, (wchar_t *) L"\n}(modules, window));\n");
  return string;
}