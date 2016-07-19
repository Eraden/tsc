#include <tsc/parser.h>

static void
__attribute__(( visibility("hidden") ))
TS_append_argument(TSFunctionData *data, TSParserToken argument) {
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

static void
__attribute__(( visibility("hidden") ))
TS_parse_function_arguments(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  const char *tok;
  u_short hadStartBracket = 0;
  u_short assign = 0;
  TSFunctionData *functionData = (TSFunctionData *) token->data;

  TSParserToken argument;
  TSLocalVariableData *argumentData = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);
  argumentData->name = NULL;
  argumentData->value = NULL;
  argumentData->type = NULL;
  argument.tokenType = TS_VAR;
  argument.data = argumentData;
  argument.visibility = TS_VISIBILITY_SCOPE;
  argument.children = NULL;
  argument.childrenSize = 0;

  while (1) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of function argument", tsFile, token);
    }
    if (tok[0] == '(') {
      hadStartBracket = 1;
      *movedBy += strlen(tok);
      continue;
    } else if (!hadStartBracket) {
      ts_token_syntax_error("Function arguments starts before bracket", tsFile, token);
    }
    if (tok[0] == ')') {
      if (argumentData->name != NULL) {
        TS_append_argument(functionData, argument);
      } else {
        free(argumentData);
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
      if (argumentData->name == NULL) {
        ts_token_syntax_error("Assigning to argument without name", tsFile, token);
      }
      assign = 1;
    } else if (tok[0] == ',') {
      if (argumentData->name == NULL) {
        ts_token_syntax_error("Declared argument as next but previous has no name", tsFile, token);
      }
      TS_append_argument(functionData, argument);

      argument.tokenType = TS_VAR;
      argument.visibility = TS_VISIBILITY_SCOPE;
      argument.position = 0;
      argument.character = 0;
      argument.line = 0;
      argumentData = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);
      argumentData->name = NULL;
      argumentData->value = NULL;
      argumentData->type = NULL;
      argument.data = argumentData;
      assign = 0;
    } else if (assign) {
      if (argumentData->value != NULL) {
        char *newPointer = (char *) calloc(sizeof(char), strlen(argumentData->value) + strlen(tok) + 1);
        strcpy(newPointer, argumentData->value);
        strcat(newPointer, tok);
        free((void *) argumentData->value);
        argumentData->value = newPointer;
      } else {
        argumentData->value = TS_clone_string(tok);
      }
    } else if (tok[0] == ':') {
      if (argumentData->name == NULL) {
        ts_syntax_error("Declared argument type but argument has no name", tsFile->file, token->character, token->line);
      }
      if (argumentData->type != NULL) {
        ts_syntax_error("Missing argument type after typing symbol", tsFile->file, token->character, token->line);
      }
      free((void *) tok);
      while (1) {
        tok = (const char *) TS_getToken(tsParseData->stream);
        if (tok == NULL) {
          ts_syntax_error("Unexpected end of function arguments", tsFile->file, token->character, token->line);
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
      argumentData->type = TS_clone_string(tok);
    } else {
      if (argumentData->name == NULL) {
        argumentData->name = TS_clone_string(tok);
      } else {
        const char *old = argumentData->name;
        char *name = calloc(sizeof(char), strlen(argumentData->name) + strlen(tok) + 1);
        strcat(name, old);
        strcat(name, tok);
        name[strlen(name)] = 0;
        argumentData->name = name;
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
      ts_token_syntax_error("Unexpected end of function body", tsFile, token);
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
      u_long size = token->childrenSize + 1;
      TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), size);
      if (token->children) memcpy(newPointer, token->children, sizeof(TSParserToken) * token->childrenSize);
      if (token->children) free(token->children);
      token->children = newPointer;
      token->children[token->childrenSize] = tsParserToken;
      token->childrenSize += 1;
    } else {
      free(tsParserToken.data);
    }
  }
}

const TSParserToken TS_parse_function(TSFile *tsFile, TSParseData *tsParseData) {
  TS_TOKEN_BEGIN("function");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_FUNCTION;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  const char *tok;

  {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of function", tsFile, &token);
    }
    movedBy += strlen(tok);
    free((void *) tok);
  }

  tok = (const char *) TS_getToken(tsParseData->stream);
  TSFunctionData *functionData = calloc(sizeof(TSFunctionData), 1);
  if (functionData == NULL) exit(1);
  functionData->arguments = NULL;
  functionData->argumentsSize = 0;
  if (!TS_name_is_valid(tok)) {
    ts_token_syntax_error("Invalid function name", tsFile, &token);
  }
  movedBy += strlen(tok);
  token.data = functionData;
  functionData->name = TS_clone_string(tok);
  free((void *) tok);

  TS_parse_function_arguments(tsFile, tsParseData, &token, &movedBy);
  TS_parse_function_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("function");
  return token;
}