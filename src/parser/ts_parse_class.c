#include <tsc/parser.h>
#include <tsc/register.h>

static void
__attribute__( (visibility("hidden") ))
TS_parse_class_head(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData,
    TSParserToken *token,
    u_long *movedBy
) {
  TSClassData *data = (TSClassData *) token->data;
  unsigned char proceed = 1;
  const char *tok;
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of class header", tsFile, token);
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
        tsParseData->character = 0;
        tsParseData->line += 1;
        *movedBy = 0;
        break;
      }
      case '{': {
        *movedBy += strlen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        tsParseData->position += *movedBy;
        tsParseData->character += *movedBy;
        tsParseData->line += 1;
        tsParseData->token = tok;
        *movedBy = 0;
        TSParserToken t = TS_parse_ts_token(tsFile, tsParseData);
        switch (t.tokenType) {
          case TS_EXTENDS:
          {
            if (data->parentClass != NULL) {
              ts_token_syntax_error("Unexpected parent name. Class can have only one parent", tsFile, token);
            }
            data->parentClass = t.data;
            break;
          }
          case TS_IMPLEMENTS:
          {
            break;
          }
          case TS_UNKNOWN:
          {
            free(t.data);
            break;
          }
          default:
          {
            break;
          }
        }
        free((void *) tok);
      }
    }
  }
}

const TSParserToken TS_parse_class(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("class");
  u_long movedBy = strlen(tsParseData->token);

  TSClassData *data = (TSClassData *) calloc(sizeof(TSClassData), 1);
  TSParserToken token;
  token.tokenType = TS_CLASS;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = data;

  data->name = NULL;
  data->parentClass = NULL;
  data->implementsInterfaces = NULL;
  data->implementsInterfacesSize = 0;

  const char *tok;
  unsigned char proceed = 1;
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Missing class name", tsFile, &token);
    }
    if (tok[0] != ' ') {
      if (!TS_name_is_valid(tok)) {
        ts_token_syntax_error("Invalid class name", tsFile, &token);
      }
      data->name = TS_clone_string(tok);
      proceed = 0;
    }
  }

  TS_register_class(tsFile, token);

  TS_parse_class_head(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("class");
  return token;
}