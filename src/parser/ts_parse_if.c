#include <tsc/parser.h>

static void
__attribute__(( visibility("hidden") ))
TS_parse_if_body(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  log_to_file("->   parsing as %s body\n", "if");
  const char *tok;
  TSConditionBodyTermination termination = TS_ENDS_WITHOUT_BRACKET;

  volatile unsigned char proceed = 1;
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of if body while looking for brackets", tsFile, token);
    }

    switch (tok[0]) {
      case ' ': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '\n': {
        *movedBy += strlen(tok);
        free((void *) tok);

        tsParseData->position += *movedBy;
        tsParseData->line += 1;
        tsParseData->character = 0;
        *movedBy = 0;
        termination = TS_ENDS_WITHOUT_BRACKET;
        break;
      }
      case ';': {
        *movedBy += strlen(tok);
        free((void *) tok);
        if (termination == TS_ENDS_WITHOUT_BRACKET) {
          return;
        }
      }
      case '{': {
        *movedBy += strlen(tok);
        free((void *) tok);

        termination = TS_ENDS_WITH_BRACKET;
        proceed = 0;
        break;
      }
      default: {
        termination = TS_ENDS_WITHOUT_BRACKET;
        TS_put_back(tsParseData->stream, tok);
        proceed = 0;

        free((void *) tok);
        break;
      }
    }
  }

  proceed = 1;
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    log_to_file("(collecting if child nodes) current token: '%s'\n", tok);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of if body while collecting child nodes", tsFile, token);
    }
    switch (tok[0]) {
      case ' ': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '\n': {
        *movedBy += strlen(tok);
        tsParseData->position += *movedBy;
        tsParseData->line += 1;
        tsParseData->character = 0;
        *movedBy = 0;
        break;
      }
      case '}': {
        if (termination != TS_ENDS_WITH_BRACKET) {
          ts_token_syntax_error("Unexpected end bracket for if during collecting child nodes. Starting one was not declared", tsFile, token);
        }
        *movedBy += strlen(tok);
        free((void *) tok);

        proceed = 0;
        break;
      }
      case ';': {
        *movedBy += strlen(tok);
        free((void *) tok);

        if (termination == TS_ENDS_WITHOUT_BRACKET) {
          proceed = 0;
        }
        break;
      }
      default: {
        log_to_file("(collecting if child nodes) [default] token: '%s'\n", tok);
        tsParseData->token = tok;
        tsParseData->character += *movedBy;
        tsParseData->position += *movedBy;
        *movedBy = 0;

        TSParserToken t = TS_parse_ts_token(tsFile, tsParseData);
        TS_push_child(token, t);

        free((void *) tok);
        break;
      }
    }
  }
  log_to_file("%s\n", "(collecting if child nodes) done");
  log_to_file("->   done %s body\n", "if");
}

static void
__attribute__(( visibility("hidden") ))
TS_parse_if_condition(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  const char *tok;
  TSIfData *data = (TSIfData *) token->data;
  volatile unsigned char proceed = 1;
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of if condition", tsFile, token);
    }
    switch (tok[0]) {
      case ' ': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '\n': {
        *movedBy += strlen(tok);
        tsParseData->position += *movedBy;
        tsParseData->line += 1;
        tsParseData->character = 0;
        *movedBy = 0;
        break;
      }
      case '(': {
        *movedBy += strlen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        ts_token_syntax_error("Unexpected token after if", tsFile, token);
      }
    }
  }
  // after (

  proceed = 1;
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of if conditions", tsFile, token);
    }
    switch (tok[0]) {
      case ' ': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '\n': {
        *movedBy += strlen(tok);
        free((void *) tok);

        tsParseData->position += *movedBy;
        tsParseData->line += 1;
        tsParseData->character = 0;
        *movedBy = 0;
        break;
      }
      case ')': {
        *movedBy += strlen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        TSParserToken t;
        t.tokenType = TS_UNKNOWN;
        t.visibility = TS_VISIBILITY_PUBLIC;
        t.position = tsParseData->position + *movedBy;
        t.character = tsParseData->character + *movedBy;
        t.line = tsParseData->line;
        t.data = (void *) TS_clone_string(tok);
        t.childrenSize = 0;
        t.children = NULL;

        TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), data->conditionsSize + 1);
        if (data->conditions != NULL)
          memcpy(newPointer, data->conditions, sizeof(TSParserToken) * data->conditionsSize);
        free(data->conditions);
        data->conditions = newPointer;
        data->conditions[data->conditionsSize] = t;
        data->conditionsSize += 1;

        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
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

void TS_free_if(const TSParserToken token) {
  TS_free_children(token);

  TSIfData *data = token.data;
  if (data == NULL) return;

  if (data->conditions) {
    for (u_long conditionIndex = 0; conditionIndex < data->conditionsSize; conditionIndex++) {
      TS_free_tsToken(data->conditions[conditionIndex]);
    }
    free(data->conditions);
  }
  free(data);
}
