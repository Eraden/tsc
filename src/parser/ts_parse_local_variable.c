#include <tsc/parser.h>

static void
__attribute__((visibility("hidden")))
TS_parse_local_variable_body(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token,
    u_long *movedBy
) {
  const char *tok;
  volatile unsigned char process = 1;
  TSVariableParseFlag parseFlag = TS_PARSE_VARIABLE_NAME;
  TSLocalVariableData *data = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);

  data->name = NULL;
  data->value = NULL;
  data->type = NULL;
  token->data = data;

  while (process) {
    tok = (const char *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      switch (parseFlag) {
        case TS_PARSE_VARIABLE_NAME:
          ts_token_syntax_error("Missing variable name", tsFile, token);
        case TS_PARSE_VARIABLE_TYPE:
          ts_token_syntax_error("Expect variable type but none provided", tsFile, token);
        case TS_PARSE_VARIABLE_VALUE:
          ts_token_syntax_error("Expect variable default value but none provided", tsFile, token);
        case TS_PARSE_VARIABLE_NONE:
        default:
          return;
      }
    }
    switch (tok[0]) {
      case '\n': {
        free((void *) tok);
        ts_token_syntax_error("New line in variable definition is not allowed!", tsFile, token);
      }
      case ' ': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case ':': {
        parseFlag = TS_PARSE_VARIABLE_TYPE;
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '=': {
        parseFlag = TS_PARSE_VARIABLE_VALUE;
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case ';': {
        process = 0;
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      default: {
        if (parseFlag == TS_PARSE_VARIABLE_NAME) {
          data->name = TS_clone_string(tok);
          *movedBy += strlen(tok);
          free((void *) tok);
          parseFlag = TS_PARSE_VARIABLE_NONE;

        } else if (parseFlag == TS_PARSE_VARIABLE_VALUE) {
          log_to_file("%s", "Setting value for local variable\n");

          u_long size = TS_STRING_END + strlen(tok);
          if (data->value) size += strlen(data->value);
          char *newPointer = (char *) calloc(sizeof(char), size);
          if (data->value != NULL) strcpy(newPointer, data->value);
          if (data->value != NULL) free((void *) data->value);
          strcat(newPointer, tok);
          data->value = newPointer;
          log_to_file("    Local variable current value: '%s'\n", data->value);

          *movedBy += strlen(tok);
          free((void *) tok);

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

          *movedBy += strlen(tok);
          free((void *) tok);

        } else {
          free((void *) tok);
          ts_token_syntax_error("Unexpected token while parsing local variable\n", tsFile, token);
        }
        break;
      }
    }
  }
}

const TSParserToken
TS_parse_var(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("var");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_VAR;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.position = tsParseData->position;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  TS_parse_local_variable_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("var");
  return token;
}

const TSParserToken
TS_parse_let(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("let");

  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_LET;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  TS_parse_local_variable_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("let");
  return token;
}

const TSParserToken
TS_parse_const(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("const");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_CONST;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  TS_parse_local_variable_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("const");
  return token;
}

void
TS_free_var(
    TSParserToken token
) {
  TS_free_children(token);

  TSLocalVariableData *data = token.data;
  if (data == NULL) return;
  if (data->name != NULL) free((void *) data->name);
  if (data->type != NULL) free((void *) data->type);
  if (data->value != NULL) free((void *) data->value);
  free(data);
}

void
TS_free_let(
    TSParserToken token
) {
  TSLocalVariableData *data = token.data;
  if (data == NULL) return;
  if (data->name != NULL) free((void *) data->name);
  if (data->type != NULL) free((void *) data->type);
  if (data->value != NULL) free((void *) data->value);
  free(data);
}

void
TS_free_const(
    TSParserToken token
) {
  TSLocalVariableData *data = token.data;
  if (data == NULL) return;
  if (data->name != NULL) free((void *) data->name);
  if (data->type != NULL) free((void *) data->type);
  if (data->value != NULL) free((void *) data->value);

  free(data);
}
