#include <tsc/parser.h>

static void TS_parse_if_body(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  log_to_file("->   parsing as %s body\n", "if");
  const char *tok;
  TSConditionBodyTermination termination;

  while (1) {
    tok = (const char *) TS_getToken(tsParseData->stream);
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
    tok = (const char *) TS_getToken(tsParseData->stream);
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
        TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), token->childrenSize + 1);
        memcpy(newPointer, token->children, token->childrenSize * sizeof(TSParserToken));
        free(token->children);
        token->children = newPointer;
        token->children[token->childrenSize] = t;
        token->childrenSize += 1;
      }
      free((void *) tok);
    }
  }
  log_to_file("->   done %s body\n", "if");
}

static void TS_parse_if_condition(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  const char *tok;
  TSIfData *data = (TSIfData *) token->data;
  while (1) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_syntax_error("Unexpected end of if condition", tsFile->file, tsParseData->line, tsParseData->character);
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
      ts_syntax_error("Unexpected token after if", tsFile->file, tsParseData->line, tsParseData->character);
    }
  }
  // after (

  while (1) {
    tok = (const char *) TS_getToken(tsParseData->stream);
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
      // TODO Implement
      TSParserToken t;
      t.position = tsParseData->position + *movedBy;
      t.character = tsParseData->character + *movedBy;
      t.line = tsParseData->line;
      t.data = (void *) TS_clone_string(tok);

      TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), data->conditionsSize + 1);
      if (data->conditions != NULL)
        memcpy(newPointer, data->conditions, sizeof(TSParserToken) * data->conditionsSize);
      free(data->conditions);
      data->conditions = newPointer;
      data->conditions[data->conditionsSize] = t;
      data->conditionsSize += 1;

      *movedBy += strlen(tok);
      free((void *) tok);
    }
  }
}

const TSParserToken TS_parse_if(TSFile *tsFile, TSParseData *tsParseData) {
  TS_TOKEN_BEGIN("if")
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_IF;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  TSIfData *data = (TSIfData *) calloc(sizeof(TSIfData), 1);
  data->conditions = NULL;
  data->conditionsSize = 0;
  token.data = data;

  TS_parse_if_condition(tsFile, tsParseData, &token, &movedBy);
  TS_parse_if_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  TS_TOKEN_END("if")
  return token;
}