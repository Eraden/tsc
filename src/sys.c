#include <tsc/sys.h>
#include <tsc/log.h>

void ts_syntax_error(const char *msg, const char *file, const u_long line, const u_long character) {
  log_error("Syntax error: %s\n", msg);
  ts_log_position(file, line, character);
  exit(2);
}

void ts_log_position(const char *file, const u_long character, const u_long line) {
  log_error("      Position: %s:%lu:%lu\n", file, line, character);
}