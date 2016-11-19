#include <cts/highlight.h>

void TS_highlight(const char *color, FILE *file) {
  fprintf(file, "%s", color);
}

void TS_clear_highlight() {
  fwprintf(stdout, L"%s", TS_COLOR_NORMAL);
  fwprintf(stderr, L"%s", TS_COLOR_NORMAL);
}

void TS_highlight_code(const char *color) {
  TS_highlight(color, stdout);
}

void TS_highlight_error() {
  TS_highlight(TS_COLOR_RED, stderr);
}
