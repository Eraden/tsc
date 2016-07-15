#include <tsc/parser.h>

static void TS_append_argument(TSFunctionData *data, TSParserToken argument) {
  if (data == NULL) return;
  if (data->arguments == NULL) {
    data->arguments = (TSParserToken *) calloc(sizeof(TSParserToken), 1);
    data->arguments[0] = argument;
    data->argumentsSize = 1;
  } else {
    TSParserToken *newPointer = calloc(sizeof(TSParserToken), data->argumentsSize + 1);
    memcpy(newPointer, data->arguments, data->argumentsSize * sizeof(TSParserToken));
    free(data->arguments);
    data->arguments = newPointer;
    data->arguments[data->argumentsSize] = argument;
    data->argumentsSize += 1;
  }
}

static void TS_parse_function_arguments(TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  const char *tok;
  u_short hadStartBracket = 0;
  u_short assign = 0;
  TSFunctionData *functionData = (TSFunctionData *) token->data;

  TSParserToken argument;
  TSLocalVariableData *tsLocalVariableData = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);
  tsLocalVariableData->name = NULL;
  tsLocalVariableData->value = NULL;
  tsLocalVariableData->type = NULL;
  argument.tokenType = TS_VAR;
  argument.data = tsLocalVariableData;
  argument.visibility = TS_VISIBILITY_SCOPE;
  argument.children = NULL;
  argument.childrenSize = 0;

  while (1) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) SYNTAX_ERROR;
    if (tok[0] == '(') {
      hadStartBracket = 1;
      *movedBy += strlen(tok);
      continue;
    } else if (!hadStartBracket) {
      SYNTAX_ERROR;
    }
    if (tok[0] == ')') {
      if (tsLocalVariableData->name != NULL) {
        TS_append_argument(functionData, argument);
      } else {
        free(tsLocalVariableData);
      }
      *movedBy += strlen(tok);
      free((void *) tok);
      break;
    }
    if (tok[0] == ' ') {
      continue;
    } else if (tok[0] == '\n') {
      tsParseData->line += 1;
      tsParseData->character = 0;
      tsParseData->position += *movedBy;
      *movedBy = 0;
    } else if (strcmp(tok, "private") == 0) {
      argument.visibility = TS_VISIBILITY_PRIVATE;
    } else if (strcmp(tok, "protected") == 0) {
      argument.visibility = TS_VISIBILITY_PROTECTED;
    } else if (strcmp(tok, "public") == 0) {
      argument.visibility = TS_VISIBILITY_PUBLIC;
    } else if (tok[0] == '=') {
      if (tsLocalVariableData->name == NULL) {
        SYNTAX_ERROR;
      }
      assign = 1;
    } else if (tok[0] == ',') {
      if (tsLocalVariableData->name == NULL) {
        SYNTAX_ERROR;
      }
      TS_append_argument(functionData, argument);

      argument.tokenType = TS_VAR;
      argument.visibility = TS_VISIBILITY_SCOPE;
      argument.position = 0;
      argument.character = 0;
      argument.line = 0;
      tsLocalVariableData = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);
      tsLocalVariableData->name = NULL;
      tsLocalVariableData->value = NULL;
      tsLocalVariableData->type = NULL;
      argument.data = tsLocalVariableData;
      assign = 0;
    } else if (assign) {
      tsLocalVariableData->value = TS_clone_string(tok);
      const char *old = tsLocalVariableData->value;
      char *value = calloc(sizeof(char), strlen(tsLocalVariableData->value) + strlen(tok) + 1);
      strcat(value, old);
      strcat(value, tok);
      value[strlen(value)] = 0;
      tsLocalVariableData->value = value;
      free((void *) old);
    } else if (tok[0] == ':') {
      if (tsLocalVariableData->name == NULL) {
        SYNTAX_ERROR;
      }
      if (tsLocalVariableData->type != NULL) {
        SYNTAX_ERROR;
      }
      free((void *) tok);
      while (1) {
        tok = (const char *) TS_getToken(tsParseData->stream);
        if (tok == NULL) {
          SYNTAX_ERROR;
        }
        if (tok[0] == ' ') {
          *movedBy += strlen(tok);
          free((void *) tok);
        } else if (tok[0] == '\n') {
          tsParseData->line += 1;
          tsParseData->character = 0;
          tsParseData->position += *movedBy;
          *movedBy = 0;
        } else {
          break;
        }
      }
      tsLocalVariableData->type = TS_clone_string(tok);
    } else {
      if (tsLocalVariableData->name == NULL) {
        tsLocalVariableData->name = TS_clone_string(tok);
      } else {
        const char *old = tsLocalVariableData->name;
        char *name = calloc(sizeof(char), strlen(tsLocalVariableData->name) + strlen(tok) + 1);
        strcat(name, old);
        strcat(name, tok);
        name[strlen(name)] = 0;
        tsLocalVariableData->name = name;
        free((void *) old);
      }
      argument.position = token->position + *movedBy;
      argument.character = tsParseData->character + *movedBy;
      argument.line = tsParseData->line;
    }
    *movedBy += strlen(tok);
    free((void *) tok);
  }
}

static void TS_parse_function_body(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  const char *tok = NULL;
  // move to bracket '{'
  while (1) {
    tok = (const char *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      SYNTAX_ERROR;
    }
    *movedBy += strlen(tok);
    if (tok[0] == '{') {
      *movedBy += strlen(tok);
      free((void *) tok);
      break;
    } else if (tok[0] == '\n') {
      tsParseData->line += 1;
      tsParseData->character = 0;
      tsParseData->position += *movedBy;
      *movedBy = 0;
    }
  }

  while (1) {
    tok = (const char *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_syntax_error("Unexpected end of function body", tsFile->file, tsParseData->character, tsParseData->line);
    }
    else if (tok[0] == '}') {
      *movedBy += strlen(tok);
      free((void *) tok);
      break;
    }
    else if (tok[0] == '\n') {
      *movedBy += strlen(tok);
      free((void *) tok);

      tsParseData->line += 1;
      tsParseData->character = 0;
      tsParseData->position += *movedBy;
      *movedBy = 0;

      continue;
    }
    else if (tok[0] == ' ') {
      *movedBy += strlen(tok);
      free((void *) tok);

      continue;
    }

    tsParseData->token = TS_clone_string(tok);
    *movedBy += strlen(tok);
    free((void *) tok);
    TSParserToken tsParserToken = TS_parse_ts_token(tsFile, tsParseData);
    free((void *) tsParseData->token);
    if (tsParserToken.tokenType != TS_UNKNOWN) {
      if (token->children == NULL) {
        token->children = (TSParserToken *) calloc(sizeof(TSParserToken), 1);
        token->children[0] = tsParserToken;
        token->childrenSize = 1;
      } else {
        TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), token->childrenSize + 1);
        memcpy(newPointer, token->children, token->childrenSize * sizeof(TSParserToken));
        free(token->children);
        token->children = newPointer;
        token->children[token->childrenSize] = tsParserToken;
        token->childrenSize += 1;
      }
    } else {
    }
  }
}

const TSParserToken TS_parse_function(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "function");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_FUNCTION;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  const char *tok;

  tok = (const char *) TS_getToken(tsParseData->stream);

  if (tok == NULL) SYNTAX_ERROR;
  movedBy += strlen(tok);
  free((void *) tok);

  tok = (const char *) TS_getToken(tsParseData->stream);
  TSFunctionData *functionData = calloc(sizeof(TSFunctionData), 1);
  if (functionData == NULL) exit(1);
  functionData->arguments = NULL;
  functionData->argumentsSize = 0;
  TS_validate_name(tok);
  movedBy += strlen(tok);
  token.data = functionData;
  functionData->name = TS_clone_string(tok);
  free((void *) tok);

  TS_parse_function_arguments(tsParseData, &token, &movedBy);
  TS_parse_function_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  log_to_file("-> end %s\n", "function");
  return token;
}