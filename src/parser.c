#include <tsc/parser.h>

#define SYNTAX_ERROR { fprintf(stdout, "Syntax error\n"); exit(2); }

static void TS_put_back(FILE *stream, const char *value);
static void TS_validate_name(const char *name);

static const char *TS_clone_string(const char *string);
static void TS_append_ts_parser_token(TSFile *tsFile, TSParserToken *token);
static const TSParserToken TS_parse_var(TSFile *tsFile, TSParseData *tsParseData);
static const TSParserToken TS_parse_let(TSFile *tsFile, TSParseData *tsParseData);
static const TSParserToken TS_parse_const(TSFile *tsFile, TSParseData *tsParseData);
static const TSParserToken TS_parse_class(TSFile *tsFile, TSParseData *tsParseData);
static const TSParserToken TS_parse_function(TSFile *tsFile, TSParseData *tsParseData);
static const TSParserToken TS_parse_arrow(TSFile *tsFile, TSParseData *tsParseData);
static TSParserToken TS_parse_ts_token(TSFile *tsFile, TSParseData *data);
static void TS_next_line(TSParseData *data);

static void TS_parse_local_variable_body(TSParseData *tsParseData, u_long *movedBy, TSParserToken *token);
static void TS_parse_local_variable_value(TSParseData *tsParseData, TSLocalVariableData *varData, u_long *movedBy);

static const char *TS_clone_string(const char *string) {
  char *clone = (char *) calloc(sizeof(char), sizeof(string) + 1);
  clone[sizeof(string)] = 0;
  strcpy(clone, string);
  return clone;
}

static void TS_parse_local_variable_body(TSParseData *tsParseData, u_long *movedBy, TSParserToken *token) {
  const char *white = TS_getToken(tsParseData->stream);
  if (white == NULL)
    SYNTAX_ERROR;
  (*movedBy) += strlen(white);
  free((void *) white);

  const char *name = TS_getToken(tsParseData->stream);
  TSLocalVariableData *varData = calloc(sizeof(TSLocalVariableData), 1);
  if (varData == NULL)
    SYNTAX_ERROR;
  TS_validate_name(name);
  (*movedBy) += strlen(name);
  varData->name = name;
  (*token).data = varData;

  TS_parse_local_variable_value(tsParseData, varData, movedBy);
}

static void TS_parse_local_variable_value(TSParseData *tsParseData, TSLocalVariableData *varData, u_long *movedBy) {
  const char *tok = NULL;
  while (1) {
    tok = TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      break;
    } else if (tok[0] == ' ' || tok[0] == ';') {
      *movedBy += strlen(tok);
      free((void *) tok);
    } else if (tok[0] == '=') {
      *movedBy += strlen(tok);
      free((void *) tok);
      while (1) {
        tok = TS_getToken(tsParseData->stream);
        if (tok == NULL) SYNTAX_ERROR;
        if (tok[0] == '\n') SYNTAX_ERROR;
        if (tok[0] != ' ') {
          break;
        } else {
          *movedBy += strlen(tok);
          free((void *) tok);
        }
      }
      varData->value = tok;
      *movedBy += strlen(tok);
      break;
    } else {
      TS_put_back(tsParseData->stream, tok);
      free((void *) tok);
      break;
    }
  }
}

static const TSParserToken TS_parse_var(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "var");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_VAR;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.position = tsParseData->position;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  TS_parse_local_variable_body(tsParseData, &movedBy, &token);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  log_to_file("-> end %s\n", "var");
  return token;
}

static const TSParserToken TS_parse_let(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "let");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_LET;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  TS_parse_local_variable_body(tsParseData, &movedBy, &token);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  log_to_file("-> end %s\n", "let");
  return token;
}

static const TSParserToken TS_parse_const(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "const");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_CONST;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  TS_parse_local_variable_body(tsParseData, &movedBy, &token);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  log_to_file("-> end %s\n", "const");
  return token;
}

static const TSParserToken TS_parse_class(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "class");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_CLASS;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  log_to_file("-> end %s\n", "class");
  return token;
}

static void TS_append_argument(TSFunctionData *functionData, TSParserToken argument) {
  if (functionData->arguments == NULL) {
    functionData->arguments = (TSParserToken *) calloc(sizeof(TSParserToken), 1);
    functionData->arguments[0] = argument;
    functionData->argumentsSize = 1;
  } else {
    functionData->arguments = (TSParserToken *) calloc(sizeof(TSParserToken), functionData->argumentsSize + 1);
    functionData->arguments[functionData->argumentsSize] = argument;
    functionData->argumentsSize += 1;
  }
}

static void TS_parse_function_arguments(TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  const char *tok;
  u_short hadStartBracket = 0;
  u_short assign = 0;
  TSFunctionData *functionData = (TSFunctionData *) token->data;

  TSParserToken argument;
  TSLocalVariableData *data = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);
  argument.tokenType = TS_VAR;
  argument.data = data;
  argument.visibility = TS_VISIBILITY_SCOPE;
  argument.children = NULL;
  argument.childrenSize = 0;

  while (1) {
    tok = TS_getToken(tsParseData->stream);
    if (tok == NULL) SYNTAX_ERROR;
    if (tok[0] == '(') {
      hadStartBracket = 1;
      *movedBy += strlen(tok);
      continue;
    } else if (!hadStartBracket) {
      SYNTAX_ERROR;
    }
    if (tok[0] == ')') {
      if (data->name != NULL) {
        TS_append_argument(functionData, argument);
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
      if (data->name == NULL) {
        SYNTAX_ERROR;
      }
      assign = 1;
    } else if (tok[0] == ',') {
      if (data->name == NULL) {
        SYNTAX_ERROR;
      }
      TS_append_argument(functionData, argument);

      argument.tokenType = TS_VAR;
      argument.visibility = TS_VISIBILITY_SCOPE;
      argument.position = 0;
      argument.character = 0;
      argument.line = 0;
      data = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);
      argument.data = data;
      assign = 0;
    } else if (assign) {
      data->value = TS_clone_string(tok);
    } else {
      data->name = TS_clone_string(tok);
      argument.position = token->position + *movedBy;
      argument.character = tsParseData->character + *movedBy;
      argument.line = tsParseData->line;
    }
    *movedBy += strlen(tok);
    free((void *) tok);
  }
}

static void TS_parse_function_body(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  const char *tok;
  // move to bracket '{'
  tok = NULL;
  while (1) {
    if (tok != NULL) {
      *movedBy += strlen(tok);
      free((void *) tok);
    }
    tok = TS_getToken(tsParseData->stream);
    if (tok == NULL) SYNTAX_ERROR;
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
    tok = TS_getToken(tsParseData->stream);
    if (tok == NULL)
      SYNTAX_ERROR;
    if (tok[0] == '}') {
      *movedBy += strlen(tok);
      free((void *) tok);
      break;
    }

    if (tok[0] == '\n') {
      *movedBy += strlen(tok);
      free((void *) tok);

      tsParseData->line += 1;
      tsParseData->character = 0;
      tsParseData->position += *movedBy;
      *movedBy = 0;
      continue;
    }
    if (tok[0] == ' ') {
      *movedBy += strlen(tok);
      free((void *) tok);

      continue;
    }

    tsParseData->token = tok;
    TSParserToken tsParserToken = TS_parse_ts_token(tsFile, tsParseData);
    if (tsParserToken.tokenType != TS_UNKNOWN) {
      if (token->children == NULL) {
        token->children = (TSParserToken *) calloc(sizeof(TSParserToken), 1);
        token->children[0] = tsParserToken;
        token->childrenSize = 1;
      } else {
        token->children = (TSParserToken *) realloc(token->children, token->childrenSize +1);
        token->children[token->childrenSize] = tsParserToken;
        token->childrenSize += 1;
      }
    }
  }
}

static const TSParserToken TS_parse_function(TSFile *tsFile, TSParseData *tsParseData) {
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

  const char *white = TS_getToken(tsParseData->stream);
  if (white == NULL) SYNTAX_ERROR;
  movedBy += strlen(white);
  free((void *) white);

  const char *name = TS_getToken(tsParseData->stream);
  TSFunctionData *functionData = calloc(sizeof(TSFunctionData), 1);
  if (functionData == NULL) exit(1);
  functionData->arguments = NULL;
  functionData->argumentsSize = 0;
  TS_validate_name(name);
  movedBy += strlen(name);
  functionData->name = name;
  token.data = functionData;

  TS_parse_function_arguments(tsParseData, &token, &movedBy);
  TS_parse_function_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  log_to_file("-> end %s\n", "function");
  return token;
}

static const TSParserToken TS_parse_arrow(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "arrow");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_ARROW;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  log_to_file("-> end %s\n", "arrow");
  return token;
}

static void TS_parse_if_body(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  log_to_file("->   parsing as %s body\n", "if");
  const char *tok;
  TSConditionBodyTermination termination;

  while (1) {
    tok = TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      SYNTAX_ERROR;
    }
    if (tok[0] == ' ') {
      *movedBy += strlen(tok);
      free((void *) tok);
    }
    else if (tok[0] == '\n') {
      *movedBy += strlen(tok);
      tsParseData->position += *movedBy;
      tsParseData->line += 1;
      tsParseData->character = 0;
      *movedBy = 0;
      termination = TS_ENDS_WITHOUT_BRACKET;
      break;
    } else {
      if (tok[0] == '{')
        termination = TS_ENDS_WITH_BRACKET;
      else
        termination = TS_ENDS_WITHOUT_BRACKET;
      *movedBy += strlen(tok);
      free((void *) tok);
      break;
    }
  }

  while (1) {
    tok = TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      SYNTAX_ERROR;
    }
    if (tok[0] == ' ') {
      *movedBy += strlen(tok);
      free((void *) tok);
    }
    else if (tok[0] == '\n') {
      *movedBy += strlen(tok);
      tsParseData->position += *movedBy;
      tsParseData->line += 1;
      tsParseData->character = 0;
      *movedBy = 0;
    } else if (tok[0] == '}') {
      if (termination != TS_ENDS_WITH_BRACKET) {
        SYNTAX_ERROR;
      }
      *movedBy += strlen(tok);
      free((void *) tok);

      break;
    } else if (tok[0] == ';') {
      *movedBy += strlen(tok);
      free((void *) tok);

      if (termination == TS_ENDS_WITHOUT_BRACKET) {
        break;
      }
    } else {
      tsParseData->token = tok;
      tsParseData->character += *movedBy;
      tsParseData->position += *movedBy;
      *movedBy = 0;
      TSParserToken t = TS_parse_ts_token(tsFile, tsParseData);
      if (token->children == NULL) {
        token->children = (TSParserToken *) calloc(sizeof(TSParserToken), 1);
        token->children[0] = t;
        token->childrenSize = 1;
      } else {
        token->children = (TSParserToken *) realloc(token->children, token->childrenSize + 1);
        token->children[token->childrenSize] = t;
        token->childrenSize += 1;
      }
      free((void *) tok);
    }
  }
  log_to_file("->   done %s body\n", "if");
}

static void TS_parse_if_condition(TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  const char *tok;
  while (1) {
    tok = TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      SYNTAX_ERROR;
    }
    if (tok[0] == ' ') {
      *movedBy += strlen(tok);
      free((void *) tok);
    }
    else if (tok[0] == '\n') {
      *movedBy += strlen(tok);
      tsParseData->position += *movedBy;
      tsParseData->line += 1;
      tsParseData->character = 0;
      *movedBy = 0;
    } else if (tok[0] == '(') {
      *movedBy += strlen(tok);
      free((void *) tok);
      break;
    } else {
      SYNTAX_ERROR;
    }
  }
  // after (

  while (1) {
    tok = TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      SYNTAX_ERROR;
    }
    if (tok[0] == ' ') {
      *movedBy += strlen(tok);
      free((void *) tok);
    }
    else if (tok[0] == '\n') {
      *movedBy += strlen(tok);
      free((void *) tok);

      tsParseData->position += *movedBy;
      tsParseData->line += 1;
      tsParseData->character = 0;
      *movedBy = 0;
    } else if (tok[0] == ')') {
      *movedBy += strlen(tok);
      free((void *) tok);

      break;
    } else {
      TSParserToken t;
      t.position = tsParseData->position + *movedBy;
      t.character = tsParseData->character + *movedBy;
      t.line = tsParseData->line;
      *movedBy += strlen(tok);
      free((void *) tok);
    }
  }
}

static const TSParserToken TS_parse_if(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "if");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_IF;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  TS_parse_if_condition(tsParseData, &token, &movedBy);
  TS_parse_if_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  log_to_file("-> end %s\n", "if");
  return token;
}

static void TS_parse_else_body(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  log_to_file("->   parsing as %s body\n", "else");
  const char *tok;
  TSConditionBodyTermination termination;

  while (1) {
    tok = TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      SYNTAX_ERROR;
    }
    if (tok[0] == ' ') {
      *movedBy += strlen(tok);
      free((void *) tok);
    }
    else if (tok[0] == '\n') {
      *movedBy += strlen(tok);
      tsParseData->position += *movedBy;
      tsParseData->line += 1;
      tsParseData->character = 0;
      *movedBy = 0;
      termination = TS_ENDS_WITHOUT_BRACKET;
      break;
    } else {
      if (tok[0] == '{')
        termination = TS_ENDS_WITH_BRACKET;
      else
        termination = TS_ENDS_WITHOUT_BRACKET;
      *movedBy += strlen(tok);
      free((void *) tok);
      break;
    }
  }

  while (1) {
    tok = TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      SYNTAX_ERROR;
    }
    if (tok[0] == ' ') {
      *movedBy += strlen(tok);
      free((void *) tok);
    }
    else if (tok[0] == '\n') {
      *movedBy += strlen(tok);
      tsParseData->position += *movedBy;
      tsParseData->line += 1;
      tsParseData->character = 0;
      *movedBy = 0;
    } else if (tok[0] == '}') {
      if (termination != TS_ENDS_WITH_BRACKET) {
        SYNTAX_ERROR;
      }
      *movedBy += strlen(tok);
      free((void *) tok);

      break;
    } else if (tok[0] == ';') {
      *movedBy += strlen(tok);
      free((void *) tok);

      if (termination == TS_ENDS_WITHOUT_BRACKET) {
        break;
      }
    } else {
      tsParseData->token = tok;
      tsParseData->character += *movedBy;
      tsParseData->position += *movedBy;
      *movedBy = 0;
      TSParserToken t = TS_parse_ts_token(tsFile, tsParseData);
      if (token->children == NULL) {
        token->children = (TSParserToken *) calloc(sizeof(TSParserToken), 1);
        token->children[0] = t;
        token->childrenSize = 1;
      } else {
        token->children = (TSParserToken *) realloc(token->children, token->childrenSize + 1);
        token->children[token->childrenSize] = t;
        token->childrenSize += 1;
      }
      free((void *) tok);
    }
  }
  log_to_file("->   done %s body\n", "else");
}

static const TSParserToken TS_parse_else(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "else");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_ELSE;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  TS_parse_else_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  log_to_file("-> end %s\n", "else");
  return token;
}

static const TSParserToken TS_parse_return(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "return");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_RETURN;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  log_to_file("-> end %s\n", "return");
  return token;
}

static const TSParserToken TS_parse_component(TSFile *tsFile, TSParseData *tsParseData) {
  log_to_file("-> parsing as %s\n", "component");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_COMPONENT;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  log_to_file("-> end %s\n", "component");
  return token;
}

static const TSKeyword TS_KEYWORDS[KEYWORDS_SIZE] = {
    TS_VAR, "var", TS_parse_var,
    TS_LET, "let", TS_parse_let,
    TS_CONST, "const", TS_parse_const,
    TS_CLASS, "class", TS_parse_class,
    TS_FUNCTION, "function", TS_parse_function,
    TS_ARROW, "=>", TS_parse_arrow,
    TS_IF, "if", TS_parse_if,
    TS_ELSE, "else", TS_parse_else,
    TS_RETURN, "return", TS_parse_return,
    TS_COMPONENT, "@", TS_parse_component,
};

static void TS_put_back(FILE *stream, const char *value) {
  for (u_long i = 0, l = strlen(value); i < l; i++) {
    ungetc(value[i], stream);
  }
}

static void TS_validate_name(const char *name) {
  char c;
  for (u_long i = 0, l = strlen(name); i < l; i++) {
    c = name[i];
    switch (c) {
      case '@':
      case '\'':
      case '"':
      case '{':
      case '}':
      case '(':
      case ')':
      case ',':
      case '.':
      case '=':
      case ':':
      case ';':
      case '+':
      case '-':
      case '/':
      case '*':
      case '\\':
      case '%':
      case '\n':
      case ' ':
      {
        log_error("Invalid name \"%s\"\n", name);
        SYNTAX_ERROR;
      }
      default:break;
    }
  }
}

static void TS_append_ts_parser_token(TSFile *tsFile, TSParserToken *token) {
  if (tsFile->tokens == NULL) {
    tsFile->tokens = calloc(sizeof(TSParserToken), 1);
    tsFile->tokens[0] = *token;
    tsFile->tokensSize = 1;
  } else {
    tsFile->tokens = realloc(tsFile->tokens, sizeof(TSParserToken) * (tsFile->tokensSize + 1));
    tsFile->tokens[tsFile->tokensSize] = *token;
    tsFile->tokensSize += 1;
  }
}

static TSParserToken TS_parse_ts_token(TSFile *tsFile, TSParseData *data) {
  for (u_short i = 0; i < KEYWORDS_SIZE; i++) {
    TSKeyword k = TS_KEYWORDS[i];
    if (strcmp(data->token, k.str) == 0) {
      log_to_file("  -  data->token = \"%s\"\n  -  k.str = \"%s\"\n", data->token, k.str);
      TSParserToken token = k.callback(tsFile, data);
      return token;
    }
  }
  TSParserToken t;
  t.tokenType = TS_UNKNOWN;
  return t;
}

static void TS_next_line(TSParseData *data) {
  data->position += 1;
  data->line += 1;
  data->character = 0;
}

static u_short TS_valid_char_for_token(char c) {
  switch (c) {
    case '@':
    case '\'':
    case '"':
    case '{':
    case '}':
    case '(':
    case ')':
    case ',':
    case '.':
    case '=':
    case ':':
    case ';':
    case '+':
    case '-':
    case '/':
    case '*':
    case '!':
    case '|':
    case '&':
    case '#':
    case '\\':
    case '%':
    case '\n':
    case ' ':
      return 0;
    default:
      return 1;
  }
}

const char *TS_getToken(FILE *stream) {
  char *tok = NULL;
  char prev = 0;
  char c = 0;
  while (!feof(stream)) {
    c = (char) fgetc(stream);
    switch (c) {
      case '@':
      case '\'':
      case '"':
      case '{':
      case '}':
      case '(':
      case ')':
      case ',':
      case '.':
      case '=':
      case ':':
      case ';':
      case '+':
      case '-':
      case '/':
      case '*':
      case '!':
      case '|':
      case '&':
      case '#':
      case '\\':
      case '%':
      case '\n':
      {
        if (tok == NULL) {
          tok = (char *) calloc(sizeof(char), 1);
          tok[0] = c;
          tok[1] = 0;
          return tok;
        } else {
          ungetc(c, stream);
          log_to_file("# token: '%s'\n", tok);
          return tok;
        }
      }
      case ' ':
      {
        if (tok == NULL) {
          tok = (char *) calloc(sizeof(char), 1);
          tok[0] = c;
          tok[1] = 0;
          prev = c;
          break;
        } else if (tok[0] == ' ') {
          u_long size = strlen(tok);
          tok = (char *) realloc(tok, size + 1);
          tok[size] = c;
          tok[size + 1] = 0;
          prev = c;
          break;
        } else {
          ungetc(c, stream);
          log_to_file("# token: '%s'\n", tok);
          return tok;
        }
      }
      default: {
        if (TS_valid_char_for_token(prev)) {
          if (tok == NULL) {
            tok = (char *) calloc(sizeof(char), 1);
            tok[0] = c;
            tok[1] = 0;
          } else {
            u_long size = strlen(tok);
            tok = (char *) realloc(tok, size + 1);
            tok[size] = c;
            tok[size + 1] = 0;
          }
        } else {
          ungetc(c, stream);
          log_to_file("# token: '%s'\n", tok);
          return tok;
        }
      }
    }
  }
  log_to_file("# token: '%s'\n", tok);
  return tok;
}

const TSFile TS_parse_file(const char *file) {
  TSFile tsFile;
  tsFile.tokens = NULL;
  tsFile.file = file;

  const char *tok;
  FILE *stream = fopen(file, "r");
  if (stream == NULL) {
    return tsFile;
  }

  TSParseData data;
  data.line = 1;
  data.character = 0;
  data.position = 0;
  data.stream = stream;

  while (1) {
    tok = TS_getToken(stream);
    if (tok == NULL) break;

    data.token = tok;
    if (data.token[0] == '\n') {
      TS_next_line(&data);
    } else {
      TSParserToken token = TS_parse_ts_token(&tsFile, &data);
      if (token.tokenType != TS_UNKNOWN)
        TS_append_ts_parser_token(&tsFile, &token);
    }

    free((void *) tok);
  }
  return tsFile;
}