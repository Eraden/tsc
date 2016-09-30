#include <cts/parser.h>

static void
__attribute__(( visibility("hidden") ))
TS_parse_else_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  log_to_file((wchar_t *) L"->   parsing as %s body\n", "else");
  const wchar_t *tok;
  TSConditionBodyTermination termination = TS_ENDS_WITH_BRACKET;
  TSParserToken *token = tsParseData->parentTSToken;

  unsigned char proceed;

  proceed = TRUE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "else body");
      break;
    }

    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        break;
      }
      case L';': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);

        return;
      }
      case L'{': {
        TS_MOVE_BY(tsParseData, tok);
        proceed = FALSE;
        termination = TS_ENDS_WITH_BRACKET;
        free((void *) tok);
        break;
      }
      default: {
        proceed = FALSE;
        TS_put_back(tsParseData->stream, tok);
        termination = TS_ENDS_WITHOUT_BRACKET;
        free((void *) tok);
        break;
      }
    }
  }

  proceed = TRUE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "else body");
      break;
    }
    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L';': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);

        if (termination == TS_ENDS_WITHOUT_BRACKET) {
          proceed = FALSE;
        }
        break;
      }
      case L'}': {
        if (termination == TS_ENDS_WITH_BRACKET) {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
        } else {
          free((void *) tok);
          TS_put_back(tsParseData->stream, tok);
        }
        proceed = FALSE;
        break;
      }
      default: {
        tsParseData->token = tok;
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
  TSParserToken *token = TS_build_parser_token(TS_ELSE, tsParseData);
  TS_parse_else_body(tsFile, tsParseData);

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
