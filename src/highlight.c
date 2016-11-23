#include <cts/highlight.h>

void TS_highlight(const char *color, FILE *file) {
  fprintf(file, "%s", color);
}

void TS_clear_highlight() {
  fprintf(stdout, "%s", TS_COLOR_NORMAL);
  fprintf(stderr, "%s", TS_COLOR_NORMAL);
}

void TS_highlighted_error(const char *msg) {
  fprintf(stderr, "%s%s%s", TS_COLOR_RED, msg, TS_COLOR_NORMAL);
}

void TS_highlight_code(const char *color) {
  TS_highlight(color, stdout);
}

void TS_highlight_error() {
  TS_highlight(TS_COLOR_RED, stderr);
}
