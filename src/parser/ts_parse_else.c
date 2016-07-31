#include <tsc/parser.h>

static void
__attribute__(( visibility("hidden") ))
TS_parse_else_body(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  log_to_file("->   parsing as %s body\n", "else");
  const char *tok;
  TSConditionBodyTermination termination = TS_ENDS_WITH_BRACKET;

  unsigned char proceed;

  proceed = 1;
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of else body", tsFile, token);
    }

    switch (tok[0]) {
      case '\n': {
        *movedBy += strlen(tok);
        tsParseData->position += *movedBy;
        tsParseData->line += 1;
        tsParseData->character = 0;
        *movedBy = 0;
        break;
      }
      case ' ': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case ';': {
        *movedBy += strlen(tok);
        free((void *) tok);

        return;
      }
      case '{': {
        proceed = 0;
        termination = TS_ENDS_WITH_BRACKET;
        *movedBy += strlen(tok);
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
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of else body", tsFile, token);
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
      case ';': {
        *movedBy += strlen(tok);
        free((void *) tok);

        if (termination == TS_ENDS_WITHOUT_BRACKET) {
          proceed = 0;
        }
        break;
      }
      case '}': {
        if (termination == TS_ENDS_WITH_BRACKET) {
          *movedBy += strlen(tok);
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
        break;
      }
    }
  }
  log_to_file("->   done %s body\n", "else");
}

const TSParserToken TS_parse_else(TSFile *tsFile, TSParseData *tsParseData) {
  TS_TOKEN_BEGIN("else");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_ELSE;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  TS_parse_else_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  TS_TOKEN_END("else");
  return token;
}

void TS_free_else(const TSParserToken token) {
  TS_free_children(token);
}
