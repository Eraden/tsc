#include <tsc/parser.h>

const TSParserToken
TS_parse_caller_argument(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken token;
  token.tokenType = TS_ARGUMENT;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = (void *) TS_clone_string(tsParseData->token);

  volatile unsigned char proceed = 1;
  const wchar_t *tok;
  u_long movedBy = wcslen(tsParseData->token);

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      if (token.data == NULL)
        ts_token_syntax_error((wchar_t *) L"Unexpected end of stream while parsing caller arguments.", tsFile, &token);
      else break;
    }

    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);

        break;
      }
      case L'\n': {
        movedBy += wcslen(tok);

        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;

        free((void *) tok);
        break;
      }
      case ',':
      case ')': {
        TS_put_back(tsParseData->stream, tok);
        proceed = 0;
        free((void *) tok);
        break;
      }
      default: {
        u_long size = wcslen(tok) + 1 + TS_STRING_END;
        if (token.data != NULL) size += wcslen(token.data);
        wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
        if (token.data != NULL) wcscpy(newPointer, token.data);
        if (token.data != NULL) free(token.data);
        wcscat(newPointer, tok);
        token.data = newPointer;

        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  return token;
}

const TSParserToken
TS_parse_caller(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken token;
  token.tokenType = TS_CALLER;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = (void *) TS_clone_string(tsParseData->token);

  volatile unsigned char proceed = 1;
  const wchar_t *tok;
  u_long movedBy = wcslen(tsParseData->token);

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      if (token.data == NULL)
        ts_token_syntax_error((wchar_t *) L"Unexpected end of stream while parsing caller arguments.", tsFile, &token);
      else break;
    }

    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);

        break;
      }
      case L';': {
        movedBy += wcslen(tok);
        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      case L'\n': {
        movedBy += wcslen(tok);
        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);

        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;
        break;
      }
      case L',':
      case L'(': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L')': {
        movedBy += wcslen(tok);
        proceed = 0;
        free((void *) tok);
        break;
      }
      default: {
        movedBy += wcslen(tok);
        tsParseData->token = tok;
        TSParserToken arg = TS_parse_caller_argument(tsFile, tsParseData);
        TS_push_child(&token, arg);
        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  return token;
}

void
TS_free_caller(
    const TSParserToken token
) {
  TS_free_children(token);
  if (token.data) free(token.data);
}