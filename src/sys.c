#include <tsc/sys.h>
#include <tsc/log.h>
#include <tsc/parser.h>

void ts_token_syntax_error(const char *msg, const TSFile *tsFile, const TSParserToken *token) {
  ts_syntax_error(msg, tsFile->file, token->character, token->line);
}

void ts_syntax_error(const char *msg, const char *file, const u_long line, const u_long character) {
  log_error("Syntax error: %s\n", msg);
  ts_log_position(file, line, character);
  exit(4);
}

void ts_log_position(const char *file, const u_long character, const u_long line) {
  log_error("      Position: %s:%lu:%lu\n", file, line + 1, character);
}