#include <tsc/parser.h>

static void
__attribute__(( visibility("hidden")))
TS_append_argument(
    TSFunctionData *data,
    TSParserToken argument
) {
  if (data == NULL) return;
  TSParserToken *newPointer = calloc(sizeof(TSParserToken), data->argumentsSize + 1);
  if (data->arguments != NULL) memcpy(newPointer, data->arguments, sizeof(TSParserToken) * data->argumentsSize);
  if (data->arguments != NULL) free(data->arguments);
  data->arguments = newPointer;
  data->arguments[data->argumentsSize] = argument;
  data->argumentsSize += 1;
}

static void
__attribute__(( visibility("hidden")))
TS_parse_function_arguments(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token,
    u_long *movedBy
) {
  const char *tok;
  u_short hadStartBracket = 0;
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

  volatile unsigned char proceed = 1;
  TSFunctionParseFlag parseFlag = TS_PARSE_FN_ARG_NAME;

  while (proceed) {
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

    switch (tok[0]) {
      case ' ': {
        *movedBy += strlen(tok);
        break;
      }
      case '\n': {
        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += *movedBy;
        *movedBy = 0;
        break;
      }
      case ')': {
        if (argumentData->name != NULL) {
          TS_append_argument(functionData, argument);
        } else {
          free(argumentData);
        }
        *movedBy += strlen(tok);
        proceed = 0;
        break;
      }
      case '=': {
        if (argumentData->name == NULL) {
          ts_token_syntax_error("Assigning to argument without name", tsFile, token);
        }
        parseFlag = TS_PARSE_FN_ARG_VALUE;
        break;
      }
      case ',': {
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
        parseFlag = TS_PARSE_FN_ARG_NAME;
        break;
      }
      case ':': {
        if (argumentData->name == NULL) {
          ts_token_syntax_error("Declared argument type but argument has no name", tsFile, token);
        }
        if (argumentData->type != NULL) {
          ts_token_syntax_error("Missing argument type after typing symbol", tsFile, token);
        }
        parseFlag = TS_PARSE_FN_ARG_TYPE;

        break;
      }
      default: {
        if (strcmp(tok, "private") == 0) {
          argument.visibility = TS_VISIBILITY_PRIVATE;
        } else if (strcmp(tok, "protected") == 0) {
          argument.visibility = TS_VISIBILITY_PROTECTED;
        } else if (strcmp(tok, "public") == 0) {
          argument.visibility = TS_VISIBILITY_PUBLIC;
        } else if (parseFlag == TS_PARSE_FN_ARG_NAME) {
          u_long size = strlen(tok) + 1;
          if (argumentData->name != NULL) size += strlen(argumentData->name);
          char *newPointer = (char *) calloc(sizeof(char), size);
          if (argumentData->name != NULL) strcpy(newPointer, argumentData->name);
          if (argumentData->name != NULL) free((void *) argumentData->name);
          strcat(newPointer, tok);
          argumentData->name = newPointer;
        } else if (parseFlag == TS_PARSE_FN_ARG_VALUE) {
          u_long size = strlen(tok) + 1;
          if (argumentData->value != NULL) size += strlen(argumentData->value);
          char *newPointer = (char *) calloc(sizeof(char), size);
          if (argumentData->value != NULL) strcpy(newPointer, argumentData->value);
          if (argumentData->value) free((void *) argumentData->value);
          strcat(newPointer, tok);
          argumentData->value = newPointer;
        } else /*if (parseFlag == TS_PARSE_FN_ARG_TYPE)*/ {
          argumentData->type = TS_clone_string(tok);
        }
        argument.position = token->position + *movedBy;
        argument.character = tsParseData->character + *movedBy;
        argument.line = tsParseData->line;
        break;
      }
    }

    *movedBy += strlen(tok);
    free((void *) tok);
  }
}

static void
__attribute__((visibility("hidden")))
TS_parse_function_body(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token,
    u_long *movedBy
) {
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

  volatile unsigned char proceed = 1;

  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_syntax_error("Unexpected end of function body", tsFile->file, tsParseData->character, tsParseData->line);
    }
    switch (tok[0]) {
      case '\n': {
        *movedBy += strlen(tok);
        free((void *) tok);

        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += *movedBy;
        *movedBy = 0;
        break;
      }
      case ' ': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '}': {
        *movedBy += strlen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
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
        break;
      }
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

  { // expect white characters here
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of function", tsFile, &token);
    } else if (tok[0] != ' ') {
      ts_token_syntax_error("Expect white character after function keyword", tsFile, &token);
    }
    movedBy += strlen(tok);
    free((void *) tok);
  }

  tok = (const char *) TS_getToken(tsParseData->stream);
  TSFunctionData *functionData = calloc(sizeof(TSFunctionData), 1);

  functionData->arguments = NULL;
  functionData->argumentsSize = 0;
  functionData->name = NULL;
  functionData->returnType = NULL;

  if (!TS_name_is_valid(tok)) {
    ts_token_syntax_error("Invalid function name", tsFile, &token);
  }

  movedBy += strlen(tok);
  token.data = functionData;
  functionData->name = TS_clone_string(tok);
  free((void *) tok);

  TS_parse_function_arguments(tsFile, tsParseData, &token, &movedBy);
  // TODO check for return type
  TS_parse_function_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("function");
  return token;
}