#include <tsc/parser/ts_variable.h>

void TS_parse_local_variable_body(TSParseData *tsParseData, u_long *movedBy, TSParserToken *token, TSFile *tsFile) {
  const char *tok;
  tok = (const char *) TS_getToken(tsParseData->stream);
  if (tok == NULL) {
    ts_syntax_error("Unexpected end of variable", tsFile->file, tsParseData->line, tsParseData->character);
  } else {
    log_to_file("    local variable white token: '%s'\n", tok);
    ts_log_position(tsFile->file, tsParseData->line, tsParseData->character);
  }
  (*movedBy) += strlen(tok);
  free((void *) tok);

  tok = (const char *) TS_getToken(tsParseData->stream);
  TSLocalVariableData *data = calloc(sizeof(TSLocalVariableData), 1);
  if (data == NULL) {
    SYNTAX_ERROR;
  }
  TS_validate_name(tok);
  (*movedBy) += strlen(tok);
  data->name = TS_clone_string(tok);
  data->value = NULL;
  data->type = NULL;
  (*token).data = data;

  TS_parse_local_variable_value(tsParseData, data, movedBy);
}

void TS_parse_local_variable_value(TSParseData *tsParseData, TSLocalVariableData *varData, u_long *movedBy) {
  const char *tok = NULL;
  while (1) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      break;
    } else if (tok[0] == ' ' || tok[0] == ';') {
      *movedBy += strlen(tok);
      free((void *) tok);
    } else if (tok[0] == '=') {
      *movedBy += strlen(tok);
      free((void *) tok);
      while (1) {
        tok = (const char *) TS_getToken(tsParseData->stream);
        if (tok == NULL) SYNTAX_ERROR;
        if (tok[0] == '\n') SYNTAX_ERROR;
        if (tok[0] != ' ') {
          break;
        } else {
          *movedBy += strlen(tok);
          free((void *) tok);
        }
      }
      varData->value = TS_clone_string(tok);
      *movedBy += strlen(tok);
      free((void *) tok);
      break;
    } else {
      TS_put_back(tsParseData->stream, tok);
      free((void *) tok);
      break;
    }
  }
}