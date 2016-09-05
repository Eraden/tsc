#include <tsc/parser.h>

TSParserToken *
TS_parse_caller(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("caller")
  TSParserToken *token = TS_build_parser_token(TS_CALLER, tsParseData);
  token->visibility = TS_VISIBILITY_SCOPE;
  token->name = (void *) TS_clone_string(tsParseData->token);

  volatile unsigned char proceed = 1;
  const wchar_t *tok;
  u_long len = wcslen(tsParseData->token);
  tsParseData->position += len;
  tsParseData->character += len;

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      if (token->data == NULL)
        ts_token_syntax_error((wchar_t *) L"Unexpected end of stream while parsing caller arguments.", tsFile, token);
      else break;
    }

    switch (tok[0]) {
      case L' ': {
        len = wcslen(tok);
        tsParseData->position += len;
        tsParseData->character += len;
        free((void *) tok);

        break;
      }
      case L';': {
        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      case L'\n': {
        len = wcslen(tok);
        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);

        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += len;
        break;
      }
      case L',':
      case L'(': {
        len = wcslen(tok);
        tsParseData->position += len;
        tsParseData->character += len;
        free((void *) tok);
        break;
      }
      case L')': {
        len = wcslen(tok);
        tsParseData->position += len;
        tsParseData->character += len;
        proceed = 0;
        free((void *) tok);
        break;
      }
      default: {
        len = wcslen(tok);
        TS_put_back(tsFile->stream, tok);
        free((void *) tok);
        tsParseData->position += len;
        tsParseData->character += len;
        TSParserToken *argument = TS_parse_argument(tsFile, tsParseData);
        TS_push_child(token, argument);
        break;
      }
    }
  }

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("caller")
  return token;
}

void
TS_free_caller(
    const TSParserToken *token
) {
  TS_free_children(token);
  if (token->data) free(token->data);
  free((void *) token);
}
