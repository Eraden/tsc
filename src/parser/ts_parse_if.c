#include <cts/parser.h>

static void
__attribute__(( visibility("hidden")))
TS_parse_if_body(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token
) {
  log_to_file((wchar_t *) L"->   parsing as %s body\n", "if");
  const wchar_t *tok;
  TSConditionBodyTermination termination = TS_ENDS_WITHOUT_BRACKET;

  u_long movedBy = 0;
  volatile unsigned char proceed = 1;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of `if` body while looking for brackets", tsFile, token);
      break;
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
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error(
          (wchar_t *) L"Unexpected end of `if` body while collecting child nodes",
          tsFile,
          token
      );
      break;
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
        free((void *) tok);
        break;
      }
      case L'}': {
        if (termination != TS_ENDS_WITH_BRACKET) {
          ts_token_syntax_error(
              (wchar_t *) L"Unexpected end bracket for `if` during collecting child nodes. Starting one was not declared",
              tsFile,
              token
          );
          proceed = 0;
          break;
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
        movedBy = wcslen(tok);

        TSParserToken *t = TS_parse_ts_token(tsFile, tsParseData);
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
__attribute__(( visibility("hidden")))
TS_parse_if_conditions(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token
) {
  u_long movedBy = 0;
  const wchar_t *tok;
  TSIfData *data = token->ifData;
  volatile unsigned char proceed = 1;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of `if` conditions", tsFile, token);
      break;
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
        ts_token_syntax_error((wchar_t *) L"Unexpected token in `if` conditions", tsFile, token);
        proceed = 0;
        break;
      }
    }
  }
  // after (

  proceed = 1;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of `if` conditions", tsFile, token);
      break;
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
          ts_token_syntax_error(
              (const wchar_t *) L"Unexpected end of `if` conditions. No condition given!",
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
        TSParserToken *t = TS_build_parser_token(TS_CONDITION, tsParseData);
        t->position = tsParseData->position + movedBy;
        t->character = tsParseData->character + movedBy;
        t->name = (void *) TS_clone_string(tok);

        TSParserToken **newPointer = (TSParserToken **) calloc(sizeof(TSParserToken *), data->conditionsSize + 1);
        if (data->conditions != NULL)
          memcpy(newPointer, data->conditions, sizeof(TSParserToken *) * data->conditionsSize);
        free(data->conditions);
        data->conditions = newPointer;
        data->conditions[data->conditionsSize] = t;
        data->conditionsSize += 1;

        movedBy += wcslen(tok);
        free((void *) tok);
        tsParseData->parentTSToken = t->parent;
        break;
      }
    }
  }
  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
}

static void
__attribute__(( visibility("hidden")))
TS_lookup_else(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token
) {
  u_long movedBy = 0;
  const wchar_t *tok;
  volatile unsigned char proceed = 1;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      return;
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
        free((void *) tok);
        break;
      }
      default: {
        movedBy += wcslen(tok);
        if (wcscmp(tok, (const wchar_t *) L"else") == 0) {
          tsParseData->token = tok;
          tsParseData->position += movedBy;
          tsParseData->character += movedBy;
          movedBy = 0;

          TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);

          TS_push_child(token, child);
        } else {
          TS_put_back(tsFile->stream, tok);
        }
        free((void *) tok);
        proceed = 0;
        break;
      }
    }
  }
}

TSParserToken *
TS_parse_if(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("if")
  u_long movedBy = wcslen(tsParseData->token);

  TSIfData *data = (TSIfData *) calloc(sizeof(TSIfData), 1);
  data->conditions = NULL;
  data->conditionsSize = 0;

  TSParserToken *token = TS_build_parser_token(TS_IF, tsParseData);
  token->ifData = data;

  TS_parse_if_conditions(tsFile, tsParseData, token);
  TS_parse_if_body(tsFile, tsParseData, token);
  TS_lookup_else(tsFile, tsParseData, token);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("if")
  return token;
}

void
TS_free_if(
    const TSParserToken *token
) {
  TS_free_children(token);

  TSIfData *data = token->ifData;
  free((void *) token);
  if (data == NULL) return;

  if (data->conditions) {
    for (u_long conditionIndex = 0; conditionIndex < data->conditionsSize; conditionIndex++) {
      TS_free_tsToken(data->conditions[conditionIndex]);
    }
    free(data->conditions);
  }
  free(data);
}
