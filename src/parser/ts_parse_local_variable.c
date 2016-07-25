#include <tsc/parser/ts_variable.h>

static void
__attribute__(( visibility("hidden") ))
TS_parse_local_variable_value(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  TSLocalVariableData *data = token->data;
  const char *tok = NULL;
  volatile unsigned char proceed = 1;
  TSVariableParseFlag parseFlag = TS_PARSE_VARIABLE_NAME;
  
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      break;
    }
    log_to_file("Token for local variable: '%s'\n", tok);
    
    switch (tok[0]) {
      case '\n': {
        ts_token_syntax_error("New line in variable definition is not allowed!", tsFile, token);
      }
      case ' ': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case ';': {
        *movedBy += strlen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      case '=': {
        log_to_file("%s", "Switching to variable value\n");
        parseFlag = TS_PARSE_VARIABLE_VALUE;
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case ':': {
        log_to_file("%s", "Switching to variable type\n");
        parseFlag = TS_PARSE_VARIABLE_TYPE;
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case ',': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      default: {
        if (parseFlag == TS_PARSE_VARIABLE_VALUE) {
          log_to_file("%s", "Setting value for local variable\n");
          u_long size = TS_STRING_END + strlen(tok);
          if (data->value) size += strlen(data->value);
          char *newPointer = (char *) calloc(sizeof(char), size);
          if (data->value != NULL) strcpy(newPointer, data->value);
          if (data->value != NULL) free((void *) data->value);
          strcat(newPointer, tok);
          data->value = newPointer;
          log_to_file("    Local variable current value: '%s'\n", data->value);
        } else if (parseFlag == TS_PARSE_VARIABLE_TYPE) {
          log_to_file("%s", "Setting type for local variable\n");
          u_long size = TS_STRING_END + strlen(tok);
          if (data->type) size += strlen(data->type);
          char *newPointer = (char *) calloc(sizeof(char), size);
          if (data->type != NULL) strcpy(newPointer, data->type);
          if (data->type != NULL) free((void *) data->type);
          strcat(newPointer, tok);
          data->type = newPointer;
          log_to_file("    Local variable current type: '%s'\n", data->type);
        } else {
          ts_token_syntax_error("Unexpected token while parsing local variable\n", tsFile, token);
        }
        break;
      }
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
//    ts_log_position(tsFile->file, tsParseData->line, tsParseData->character);
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