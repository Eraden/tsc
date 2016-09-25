#include <cts/parser.h>

static void
__attribute__(( visibility("hidden") ))
TS_parse_else_body(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token,
    u_long *movedBy
) {
  log_to_file((wchar_t *) L"->   parsing as %s body\n", "else");
  const wchar_t *tok;
  TSConditionBodyTermination termination = TS_ENDS_WITH_BRACKET;

  unsigned char proceed;

  proceed = 1;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of else body", tsFile, token);
      break;
    }

    switch (tok[0]) {
      case L'\n': {
        *movedBy += wcslen(tok);
        tsParseData->position += *movedBy;
        tsParseData->line += 1;
        tsParseData->character = 0;
        *movedBy = 0;
        break;
      }
      case L' ': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L';': {
        *movedBy += wcslen(tok);
        free((void *) tok);

        return;
      }
      case L'{': {
        proceed = 0;
        termination = TS_ENDS_WITH_BRACKET;
        *movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      default: {
        proceed = 0;
        TS_put_back(tsParseData->stream, tok);
        termination = TS_ENDS_WITHOUT_BRACKET;
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
      ts_token_syntax_error((wchar_t *) L"Unexpected end of else body", tsFile, token);
      break;
    }
    switch (tok[0]) {
      case L' ': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        *movedBy += wcslen(tok);
        free((void *) tok);

        tsParseData->position += *movedBy;
        tsParseData->line += 1;
        tsParseData->character = 0;
        *movedBy = 0;
        break;
      }
      case L';': {
        *movedBy += wcslen(tok);
        free((void *) tok);

        if (termination == TS_ENDS_WITHOUT_BRACKET) {
          proceed = 0;
        }
        break;
      }
      case L'}': {
        if (termination == TS_ENDS_WITH_BRACKET) {
          *movedBy += wcslen(tok);
          free((void *) tok);
        } else {
          free((void *) tok);
          TS_put_back(tsParseData->stream, tok);
        }
        proceed = 0;
        break;
      }
      default: {
        tsParseData->token = tok;
        tsParseData->character += *movedBy;
        tsParseData->position += *movedBy;
        *movedBy = 0;
        TSParserToken *t = TS_parse_ts_token(tsFile, tsParseData);
        if (token->children == NULL) {
          token->children = (TSParserToken **) calloc(sizeof(TSParserToken *), 1);
          token->children[0] = t;
          token->childrenSize = 1;
        } else {
          TSParserToken **newPointer = (TSParserToken **) calloc(sizeof(TSParserToken *), token->childrenSize + 1);
          memcpy(newPointer, token->children, token->childrenSize * sizeof(TSParserToken *));
          free(token->children);
          token->children = newPointer;
          token->children[token->childrenSize] = t;
          token->childrenSize += 1;
        }
        free((void *) tok);
        break;
      }
    }
  }
  log_to_file((wchar_t *) L"->   done %s body\n", "else");
}

TSParserToken *
TS_parse_else(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("else");
  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_ELSE, tsParseData);
  TS_parse_else_body(tsFile, tsParseData, token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("else");
  return token;
}

void
TS_free_else(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
