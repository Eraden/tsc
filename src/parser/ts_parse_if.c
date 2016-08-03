#include <tsc/parser.h>

static void
__attribute__(( visibility("hidden") ))
TS_parse_if_body(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token) {
  log_to_file((wchar_t *) L"->   parsing as %s body\n", "if");
  const wchar_t *tok;
  TSConditionBodyTermination termination = TS_ENDS_WITHOUT_BRACKET;

  u_long movedBy = 0;
  volatile unsigned char proceed = 1;
  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of if body while looking for brackets", tsFile, token);
    }

    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        movedBy += wcslen(tok);
        free((void *) tok);

        tsParseData->position += movedBy;
        tsParseData->line += 1;
        tsParseData->character = 0;
        movedBy = 0;
        termination = TS_ENDS_WITHOUT_BRACKET;
        break;
      }
      case L';': {
        movedBy += wcslen(tok);
        free((void *) tok);
        if (termination == TS_ENDS_WITHOUT_BRACKET) {
          return;
        }
      }
      case L'{': {
        movedBy += wcslen(tok);
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
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of if body while collecting child nodes", tsFile, token);
    }
    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        movedBy += wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->line += 1;
        tsParseData->character = 0;
        movedBy = 0;
        break;
      }
      case L'}': {
        if (termination != TS_ENDS_WITH_BRACKET) {
          ts_token_syntax_error((wchar_t *) L"Unexpected end bracket for if during collecting child nodes. Starting one was not declared", tsFile, token);
        }
        movedBy += wcslen(tok);
        free((void *) tok);

        proceed = 0;
        break;
      }
      case L';': {
        movedBy += wcslen(tok);
        free((void *) tok);

        if (termination == TS_ENDS_WITHOUT_BRACKET) {
          proceed = 0;
        }
        break;
      }
      default: {
        tsParseData->token = tok;
        tsParseData->character += movedBy;
        tsParseData->position += movedBy;
        movedBy = 0;

        TSParserToken t = TS_parse_ts_token(tsFile, tsParseData);
        TS_push_child(token, t);

        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  log_to_file((wchar_t *) L"->   done %s body\n", "if");
}

static void
__attribute__(( visibility("hidden") ))
TS_parse_if_conditions(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token) {
  u_long movedBy = 0;
  const wchar_t *tok;
  TSIfData *data = (TSIfData *) token->data;
  volatile unsigned char proceed = 1;
  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of if condition", tsFile, token);
    }
    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        movedBy += wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->line += 1;
        tsParseData->character = 0;
        movedBy = 0;
        break;
      }
      case L'(': {
        movedBy += wcslen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        ts_token_syntax_error((wchar_t *) L"Unexpected token after if", tsFile, token);
      }
    }
  }
  // after (

  proceed = 1;
  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of if conditions", tsFile, token);
    }
    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        movedBy += wcslen(tok);
        free((void *) tok);

        tsParseData->position += movedBy;
        tsParseData->line += 1;
        tsParseData->character = 0;
        movedBy = 0;
        break;
      }
      case L')': {
        if (data->conditionsSize == 0) {
          free((void *) tok);
          ts_token_syntax_error(
              (const wchar_t *) L"Unexpected end of `if`. No condition given!",
              tsFile,
              token
          );
        }
        movedBy += wcslen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        TSParserToken t;
        t.tokenType = TS_CONDITION;
        t.visibility = TS_VISIBILITY_PUBLIC;
        t.position = tsParseData->position + movedBy;
        t.character = tsParseData->character + movedBy;
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

        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
    }
  }
  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
}

const TSParserToken TS_parse_if(TSFile *tsFile, TSParseData *tsParseData) {
  TS_TOKEN_BEGIN("if")
  u_long movedBy = wcslen(tsParseData->token);

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

  TS_parse_if_conditions(tsFile, tsParseData, &token);
  TS_parse_if_body(tsFile, tsParseData, &token);

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
