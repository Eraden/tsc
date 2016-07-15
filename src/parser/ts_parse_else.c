#include <tsc/parser.h>

static void TS_parse_else_body(TSFile *tsFile, TSParseData *tsParseData, TSParserToken *token, u_long *movedBy) {
  log_to_file("->   parsing as %s body\n", "else");
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
  log_to_file("->   done %s body\n", "else");
}

const TSParserToken TS_parse_else(TSFile *tsFile, TSParseData *tsParseData) {
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