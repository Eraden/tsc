#include <tsc/parser/ts_variable.h>

static void
__attribute__(( visibility("hidden") ))
TS_parse_local_variable_value(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  TSLocalVariableData *data = token->data;
  const char *tok = NULL;
  while (1) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      break;
    } else if (tok[0] == ';') {
      *movedBy += strlen(tok);
      free((void *) tok);
      return;
    } else if (tok[0] == ' ') {
      *movedBy += strlen(tok);
      free((void *) tok);
    } else if (tok[0] == '=') {
      *movedBy += strlen(tok);
      free((void *) tok);
      while (1) {
        tok = (const char *) TS_getToken(tsParseData->stream);
        if (tok == NULL) {
          ts_syntax_error("Invalid variable value", tsFile->file, tsParseData->character, tsParseData->line);
        }
        if (tok[0] == '\n') {
          ts_syntax_error("Invalid variable value", tsFile->file, tsParseData->character, tsParseData->line);
        }
        if (tok[0] != ' ') {
          break;
        } else {
          *movedBy += strlen(tok);
          free((void *) tok);
        }
      }
      data->value = TS_clone_string(tok);
      *movedBy += strlen(tok);
      free((void *) tok);
      while (1) {
        tok = (const char *) TS_getToken(tsParseData->stream);
        if (tok == NULL || tok[0] == ',' || tok[0] == '\n' || tok[0] == ';') {
          if (tok != NULL) {
            *movedBy += strlen(tok);
            free((void *) tok);
          }
          break;
        } else {
          char *newPointer = (char *) calloc(sizeof(char), strlen(data->value) + strlen(tok) + 1);
          strcpy(newPointer, data->value);
          strcat(newPointer, tok);
          free((void *) data->value);
          data->value = newPointer;

          *movedBy += strlen(tok);
          free((void *) tok);
        }
      }
      break;
    } else {
      TS_put_back(tsParseData->stream, tok);
      free((void *) tok);
      break;
    }
  }
}

void TS_parse_local_variable_body(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
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
    log_error("%s\n", "Allocation error for local variable");
  }
  if (!TS_name_is_valid(tok)) {
    ts_syntax_error("Invalid variable name", tsFile->file, token->character, token->line);
  }
  (*movedBy) += strlen(tok);
  data->name = TS_clone_string(tok);
  data->value = NULL;
  data->type = NULL;
  (*token).data = data;

  TS_parse_local_variable_value(tsFile, tsParseData, token, movedBy);
}